use battery::{units::ratio::ratio, Manager as BatteryManager, State as BatteryState};
use chrono::{DateTime, Datelike, Local, Timelike};
use crossterm::{style::Stylize, terminal::size as terminal_size};
use git2::{
    Repository as Git2Repository, RepositoryState as Git2RepositoryState, Status as Git2Status,
};
use libc::{
    access, closedir, dirent, opendir, readdir, statvfs, strlen, DT_BLK, DT_CHR, DT_DIR, DT_FIFO,
    DT_LNK, DT_SOCK, W_OK,
};
#[cfg(target_os = "macos")]
use libc::{lstat, stat, UF_HIDDEN};
use local_ip_address::local_ip;
use num_format::{Locale, ToFormattedString};
use open::that as open_in_workspace;
#[cfg(target_os = "macos")]
use std::mem::zeroed;
use std::{
    env::{args, current_dir as std_current_directory, var as environment_variable},
    ffi::c_char,
    fmt::{Display, Error as FormatError, Formatter},
    fs::read_to_string,
    net::IpAddr,
    path::{Path, PathBuf},
    process::ExitCode,
};

const SOFTWARE_NAME: &str = env!("CARGO_PKG_NAME");
const SOFTWARE_VERSION: &str = env!("CARGO_PKG_VERSION");
const SOFTWARE_REPOSITORY_URL: &str = env!("CARGO_PKG_HOMEPAGE");
const SOFTWARE_LICENSE: &str = concat!(env!("CARGO_PKG_LICENSE"), " License");
const SOFTWARE_AUTHOR_NAME: &str = "Sherman Rofeman";
const SOFTWARE_AUTHOR_EMAIL: &str = "skippyr.developer@icloud.com";
const SOFTWARE_CREATION_YEAR: u16 = 2023;
#[cfg(target_os = "macos")]
const OS_NAME: &str = "macOS";
#[cfg(target_os = "linux")]
const OS_NAME: &str = "Linux";
#[cfg(target_arch = "aarch64")]
const CPU_ARCHITECTURE: &str = "aarch64";
#[cfg(target_arch = "x86_64")]
const CPU_ARCHITECTURE: &str = "x86_64";
const LEFT_PROMPT_SECTIONS_CONSTANT_LENGTH: u16 = 42;
const DEFAULT_BRANCH_NAME: &str = "master";
const ZSH_PERCENTAGE_SYMBOL: &str = "%%";
const ZSH_EXIT_CODE: &str = "%?";
const ZSH_TOTAL_JOBS: &str = "%j";

enum ValidationOption<T> {
    Some(T),
    None,
    Invalid,
}

#[derive(Clone, Copy)]
enum Flag {
    MainHelp,
    InitCommandHelp,
    PromptCommandHelp,
    Version,
    Repository,
}

enum Command {
    Init,
    Prompt(PromptSide),
}

enum ArgumentParseType {
    Flag(Flag),
    Command(Command),
}

#[derive(Default, Clone, Copy)]
enum PromptSide {
    #[default]
    Left,
    Right,
}

#[derive(Clone, Copy)]
enum Color {
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
}

impl Color {
    fn ansi(&self) -> u8 {
        match self {
            Self::Red => 1,
            Self::Green => 2,
            Self::Yellow => 3,
            Self::Blue => 4,
            Self::Magenta => 5,
            Self::Cyan => 6,
        }
    }
}

#[derive(Clone, Copy)]
enum DiskUsageStatus {
    Low,
    Moderate,
    High,
}

impl From<u8> for DiskUsageStatus {
    fn from(percentage: u8) -> Self {
        match percentage {
            0..60 => Self::Low,
            60..80 => Self::Moderate,
            _ => Self::High,
        }
    }
}

#[derive(Clone, Copy)]
enum BatteryChargeStatus {
    Critical,
    Low,
    Moderate,
    High,
}

impl BatteryChargeStatus {
    fn from(percentage: u8) -> Self {
        match percentage {
            0..5 => Self::Critical,
            5..30 => Self::Low,
            30..60 => Self::Moderate,
            _ => Self::High,
        }
    }
}

#[derive(Clone, Copy)]
enum DayFraction {
    Dawn,
    Morning,
    Afternoon,
    Night,
}

impl From<&DateTime<Local>> for DayFraction {
    fn from(time: &DateTime<Local>) -> Self {
        match time.hour() {
            0..6 => Self::Dawn,
            6..12 => Self::Morning,
            12..18 => Self::Afternoon,
            _ => Self::Night,
        }
    }
}

enum GitReference {
    Branch(String),
    RebaseHash(String),
}

#[derive(Clone, Copy)]
enum DirectoryEntryType {
    Directory,
    File,
    Socket,
    Fifo,
    BlockDevice,
    CharacterDevice,
    Symlink,
}

struct DiskUsage {
    percentage: u8,
    status: DiskUsageStatus,
}

struct Charge {
    percentage: u8,
    status: BatteryChargeStatus,
    is_charging: bool,
}

struct Repository {
    path: PathBuf,
    reference: GitReference,
    is_dirty: bool,
}

#[derive(Default)]
struct DirectoryEntriesCount {
    total_directories: usize,
    total_files: usize,
    total_sockets: usize,
    total_fifos: usize,
    total_block_devices: usize,
    total_character_devices: usize,
    total_symlinks: usize,
    total_hidden_entries: usize,
    total_temporary_entries: usize,
}

#[derive(Debug)]
struct Error(String);

impl Error {
    fn new<T: ToString>(message: T) -> Self {
        Self(message.to_string())
    }
}

impl Display for Error {
    fn fmt(&self, formatter: &mut Formatter) -> Result<(), FormatError> {
        write!(formatter, "{}", self.0)
    }
}

fn is_flag(argument: &str) -> bool {
    let graphemes = argument.chars().collect::<Vec<_>>();
    (graphemes.len() == 2 && graphemes[0] == '-' && graphemes[1].is_ascii_lowercase())
        || (graphemes.len() > 2
            && graphemes[0] == '-'
            && graphemes[1] == '-'
            && graphemes[2].is_ascii_lowercase())
}

fn is_malformed_flag(argument: &str) -> bool {
    argument == "-" || argument == "--"
}

fn parse_arguments() -> Result<ArgumentParseType, Error> {
    let arguments = args().skip(1).collect::<Vec<_>>();
    let mut command = ValidationOption::None;
    if let Some(argument) = arguments.first() {
        if argument == "init" {
            command = ValidationOption::Some(Command::Init);
        } else if argument == "prompt" {
            command = ValidationOption::Some(Command::Prompt(PromptSide::default()));
        } else {
            command = ValidationOption::Invalid;
        }
    }
    for argument in &arguments {
        if argument == "-h" || argument == "--help" {
            match command {
                ValidationOption::Some(Command::Init) => {
                    return Ok(ArgumentParseType::Flag(Flag::InitCommandHelp))
                }
                ValidationOption::Some(Command::Prompt(_)) => {
                    return Ok(ArgumentParseType::Flag(Flag::PromptCommandHelp))
                }
                _ => return Ok(ArgumentParseType::Flag(Flag::MainHelp)),
            }
        } else if argument == "-v" || argument == "--version" {
            return Ok(ArgumentParseType::Flag(Flag::Version));
        } else if argument == "-g" || argument == "--repo" {
            return Ok(ArgumentParseType::Flag(Flag::Repository));
        } else if is_flag(argument) {
            return Err(Error::new(format_args!(
                r#"invalid flag "{argument}" provided."#
            )));
        } else if is_malformed_flag(argument) {
            return Err(Error::new(format_args!(
                r#"malformed flag "{argument}" provided."#
            )));
        }
    }
    match command {
        ValidationOption::Some(Command::Init) => {
            return Ok(ArgumentParseType::Command(Command::Init))
        }
        ValidationOption::None => return Err(Error::new("no command provided.")),
        ValidationOption::Invalid => {
            return Err(Error::new(format_args!(
                r#"invalid command "{}" provided."#,
                arguments[0]
            )))
        }
        _ => {}
    }
    let mut prompt_side = PromptSide::Left;
    if let Some(argument) = arguments.get(1) {
        if argument == "right" {
            prompt_side = PromptSide::Right;
        } else if argument != "left" {
            return Err(Error::new(format_args!(
                r#"invalid prompt side "{argument}" provided."#
            )));
        }
    } else {
        return Err(Error::new("no prompt side provided."));
    }
    Ok(ArgumentParseType::Command(Command::Prompt(prompt_side)))
}

fn open_repository() -> Result<(), Error> {
    open_in_workspace(SOFTWARE_REPOSITORY_URL)
        .map_err(|_| Error::new("can not open the software repository."))
}

fn write_error(error: &Error) {
    eprintln!(
        "{}{}{} {} {}{} {error}",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        SOFTWARE_NAME.dark_magenta().bold(),
        "(code 1)".dark_yellow().bold(),
        ":".dark_magenta().bold()
    );
    write_info(
        &format!(
            "use {} or {} for help instructions.",
            "-h".dark_cyan(),
            "--help".dark_cyan()
        ),
        false,
    );
}

fn write_title(title: &str) {
    println!("{} {}", "❡".dark_cyan().bold(), title.dark_magenta().bold());
}

fn write_usage(command: Option<&str>, arguments: Option<&[&str]>, description: &str) {
    print!(
        "{}{}{} {} {SOFTWARE_NAME}",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
    );
    if let Some(command) = command {
        print!(" {command}");
    }
    if let Some(arguments) = arguments {
        for argument in arguments {
            print!(" <{}>", argument.dark_yellow().underlined());
        }
    }
    println!(" [{}]...", "OPTIONS".dark_yellow().underlined());
    println!("{description}");
}

fn calculate_strs_padding<'a, T: AsRef<[&'a str]>>(items: &T) -> usize {
    let mut padding = 0;
    for item in items.as_ref() {
        if item.len() > padding {
            padding = item.len();
        }
    }
    padding
}

fn write_items(title: &str, items: &[[&str; 2]]) {
    write_title(title);
    let padding = calculate_strs_padding(&items.iter().map(|item| item[0]).collect::<Vec<_>>());
    for item in items {
        println!(
            "    {}  {}",
            format!("{:padding$}", item[0]).dark_yellow(),
            item[1]
        );
    }
}

fn write_flags(flags: &[[&str; 3]]) {
    write_title("Available Options");
    let padding = calculate_strs_padding(&flags.iter().map(|opt| opt[1]).collect::<Vec<_>>());
    for flag in flags {
        println!(
            "    {}, {}  {}",
            format!("-{}", flag[0]).dark_cyan(),
            format!("--{:padding$}", flag[1]).dark_cyan(),
            flag[2]
        );
    }
}

fn write_code(code: &str, language: &str) {
    println!("    {}{}", "```".dark_grey(), language.dark_cyan());
    println!("    {code}");
    println!("    {}", "```".dark_grey());
}

fn write_info(message: &str, is_to_use_stdout: bool) {
    let message = format!("{} {message}", "  info:".dark_cyan().bold());
    if is_to_use_stdout {
        println!("{message}");
    } else {
        eprintln!("{message}");
    }
}

fn write_help() {
    write_usage(
        None,
        Some(&["COMMAND"]),
        "Performs tasks related to the River Dreams theme.",
    );
    println!();
    write_items(
        "Available Commands",
        &[
            ["init", "initiates the prompt."],
            ["prompt", "writes a prompt side."],
        ],
    );
    println!();
    write_info(
        &format!(
            "use {} or {} with each for help instructions.",
            "-h".dark_cyan(),
            "--help".dark_cyan()
        ),
        true,
    );
    println!();
    write_flags(&[
        ["h", "help", "shows the software help instructions."],
        ["v", "version", "shows the software version."],
        ["g", "repo", "opens the software repository."],
    ]);
}

fn write_init_command_help() {
    write_usage(Some("init"), None, "Initiates the prompt.");
    println!();
    println!("Use it within an eval call in ~/.zshrc:");
    write_code("eval $(river_dreams init);", "zsh");
    println!();
    write_flags(&[["h", "help", "shows the command help instructions."]]);
}

fn write_prompt_command_help() {
    write_usage(Some("prompt"), Some(&["SIDE"]), "Writes a prompt side.");
    println!();
    write_items(
        "Available Sides",
        &[
            ["left", "refers to the left prompt."],
            ["right", "refers to the right prompt."],
        ],
    );
    println!();
    write_flags(&[["h", "help", "shows the command instructions."]]);
}

fn write_version() {
    println!(
        "{} v{SOFTWARE_VERSION} {}",
        SOFTWARE_NAME.dark_magenta().bold(),
        format!("({OS_NAME} {CPU_ARCHITECTURE})").dark_grey()
    );
    println!(
        "Repository at: {}.",
        SOFTWARE_REPOSITORY_URL.dark_cyan().underlined()
    );
    println!();
    println!("{SOFTWARE_LICENSE}");
    println!(
        "Copyright (c) {SOFTWARE_CREATION_YEAR} {SOFTWARE_AUTHOR_NAME} <{}>",
        SOFTWARE_AUTHOR_EMAIL.dark_cyan().underlined()
    );
}

fn init_prompt() {
    println!("setopt promptsubst;");
    println!("export VIRTUAL_ENV_DISABLE_PROMPT=1;");
    println!("PROMPT='$(river_dreams prompt left)';");
    println!("RPROMPT='$(river_dreams prompt right)';");
}

fn calculate_number_length(mut number: u16) -> u16 {
    let mut length = if number == 0 { 1 } else { 0 };
    loop {
        if number == 0 {
            break;
        }
        length += 1;
        number /= 10;
    }
    length
}

fn disk_usage() -> Result<DiskUsage, Error> {
    let mut info = unsafe { std::mem::zeroed() };
    if unsafe { statvfs([b'/' as c_char, 0].as_ptr(), &mut info) } < 0 {
        return Err(Error::new(
            "can not retrieve info about the main disk of the computer.",
        ));
    }
    let total_bytes = info.f_frsize * info.f_blocks as u64;
    let available_bytes = info.f_frsize * info.f_bavail as u64;
    let free_bytes = total_bytes - available_bytes;
    let percentage = (free_bytes as f64 / total_bytes as f64 * 100.0) as u8;
    Ok(DiskUsage {
        percentage,
        status: DiskUsageStatus::from(percentage),
    })
}

fn battery_charge() -> Result<Option<Charge>, Error> {
    const SUPPLY_ERROR: &str = "can not retrieve info about the energy supply of the computer.";
    for battery in BatteryManager::new()
        .map_err(|_| Error::new(SUPPLY_ERROR))?
        .batteries()
        .map_err(|_| Error::new(SUPPLY_ERROR))?
    {
        let battery = match battery {
            Ok(bat) => bat,
            Err(_) => continue,
        };
        let percentage = (battery.state_of_charge().get::<ratio>() * 100.0).round() as u8;
        return Ok(Some(Charge {
            percentage,
            status: BatteryChargeStatus::from(percentage),
            is_charging: matches!(
                battery.state(),
                BatteryState::Charging | BatteryState::Full | BatteryState::Unknown
            ),
        }));
    }
    Ok(None)
}

fn color_symbol(symbol: &str, color: Option<Color>) -> String {
    match color {
        Some(color) => format!("%F{{{}}}{symbol}%f", color.ansi()),
        None => symbol.to_owned(),
    }
}

fn is_ordinal(time: &DateTime<Local>, ordinal: u32) -> bool {
    (time.day() - ordinal) % 10 == 0
}

fn day_ordinal(time: &DateTime<Local>) -> &'static str {
    if is_ordinal(time, 1) {
        "st"
    } else if is_ordinal(time, 2) {
        "nd"
    } else if is_ordinal(time, 3) {
        "rd"
    } else {
        "th"
    }
}

fn is_fs_root(path: &Path) -> bool {
    path.ancestors().count() == 1
}

fn current_directory() -> Result<PathBuf, Error> {
    if let Ok(pwd) = std_current_directory() {
        return Ok(pwd);
    } else if let Ok(pwd) = environment_variable("PWD") {
        return Ok(PathBuf::from(pwd));
    }
    Err(Error::new("can not resolve the current directory."))
}

fn can_access_current_directory() -> bool {
    unsafe { access([b'.' as c_char, 0].as_ptr(), W_OK) == 0 }
}

fn is_dot_entry(entry: &dirent) -> bool {
    (entry.d_name[0] == b'.' as c_char && entry.d_name[1] == 0)
        || (entry.d_name[0] == b'.' as c_char
            && entry.d_name[1] == b'.' as c_char
            && entry.d_name[2] == 0)
}

fn is_temporary_entry(entry: &dirent) -> bool {
    entry.d_name[unsafe { strlen(entry.d_name.as_ptr()) } - 1] == b'~' as c_char
}

fn is_hidden_entry(entry: &dirent) -> Result<bool, ()> {
    if entry.d_name[0] == b'.' as c_char {
        return Ok(true);
    }
    #[cfg(target_os = "macos")]
    {
        entry_lstat(entry).map(|stat| stat.st_flags & UF_HIDDEN != 0)
    }
    #[cfg(target_os = "linux")]
    {
        Ok(false)
    }
}

#[cfg(target_os = "macos")]
fn entry_lstat(entry: &dirent) -> Result<stat, ()> {
    let mut entry_stat = unsafe { zeroed() };
    if unsafe { lstat(entry.d_name.as_ptr(), &mut entry_stat) } < 0 {
        Err(())
    } else {
        Ok(entry_stat)
    }
}

fn entry_type(entry: &dirent) -> DirectoryEntryType {
    match entry.d_type {
        DT_DIR => DirectoryEntryType::Directory,
        DT_SOCK => DirectoryEntryType::Socket,
        DT_FIFO => DirectoryEntryType::Fifo,
        DT_BLK => DirectoryEntryType::BlockDevice,
        DT_CHR => DirectoryEntryType::CharacterDevice,
        DT_LNK => DirectoryEntryType::Symlink,
        _ => DirectoryEntryType::File,
    }
}

fn directory_entries_count() -> Result<DirectoryEntriesCount, Error> {
    let stream = unsafe { opendir([b'.' as c_char, 0].as_ptr()) };
    let mut entries_count = DirectoryEntriesCount::default();
    if stream.is_null() {
        return Err(Error::new("can not access the current directory."));
    }
    loop {
        let entry = unsafe { readdir(stream) };
        if entry.is_null() {
            break;
        }
        let entry = unsafe { *entry };
        if is_dot_entry(&entry) {
            continue;
        } else if is_temporary_entry(&entry) {
            entries_count.total_temporary_entries += 1;
        }
        match is_hidden_entry(&entry) {
            Ok(is_hidden) => {
                if is_hidden {
                    entries_count.total_hidden_entries += 1;
                }
            }
            Err(_) => continue,
        }
        match entry_type(&entry) {
            DirectoryEntryType::Directory => entries_count.total_directories += 1,
            DirectoryEntryType::Socket => entries_count.total_sockets += 1,
            DirectoryEntryType::Fifo => entries_count.total_fifos += 1,
            DirectoryEntryType::BlockDevice => entries_count.total_block_devices += 1,
            DirectoryEntryType::CharacterDevice => entries_count.total_character_devices += 1,
            DirectoryEntryType::Symlink => entries_count.total_symlinks += 1,
            DirectoryEntryType::File => entries_count.total_files += 1,
        }
    }
    unsafe {
        closedir(stream);
    }
    Ok(entries_count)
}

fn git_repository() -> Option<Repository> {
    let repository = match Git2Repository::discover(".") {
        Ok(repository) => repository,
        Err(_) => return None,
    };
    let mut reference = None;
    if let Ok(head) = repository.head() {
        if repository.state() == Git2RepositoryState::RebaseInteractive {
            if let Some(hash) = head.target() {
                reference = Some(GitReference::RebaseHash(
                    hash.to_string().chars().take(7).collect::<String>(),
                ));
            }
        } else if let Some(branch) = head.shorthand() {
            reference = Some(GitReference::Branch(String::from(branch)));
        }
    } else if let Ok(head_data) = read_to_string(repository.path().join("HEAD")) {
        if let Some(branch) = head_data.trim().strip_prefix("ref: refs/heads/") {
            reference = Some(GitReference::Branch(branch.to_string()));
        }
    } else if let Ok(configuration) = repository.config() {
        if let Ok(branch) = configuration.get_string("init.defaultBranch") {
            reference = Some(GitReference::Branch(branch));
        }
    }
    let mut is_dirty: bool = false;
    if let Ok(entries) = repository.statuses(None) {
        for entry in &entries {
            if entry.status().contains(Git2Status::IGNORED) {
                continue;
            } else if entry.status().intersects(
                Git2Status::INDEX_NEW
                    | Git2Status::INDEX_MODIFIED
                    | Git2Status::INDEX_DELETED
                    | Git2Status::INDEX_RENAMED
                    | Git2Status::INDEX_TYPECHANGE
                    | Git2Status::WT_NEW
                    | Git2Status::WT_MODIFIED
                    | Git2Status::WT_DELETED
                    | Git2Status::WT_TYPECHANGE
                    | Git2Status::WT_RENAMED,
            ) {
                is_dirty = true;
                break;
            }
        }
    }
    Some(Repository {
        path: if repository.is_bare() {
            repository.path().to_path_buf()
        } else {
            repository.workdir().unwrap().to_path_buf()
        },
        reference: match reference {
            Some(reference) => reference,
            None => GitReference::Branch(String::from(DEFAULT_BRANCH_NAME)),
        },
        is_dirty,
    })
}

fn show_when_root(symbol: &str) -> String {
    format!("%(#.{symbol}.)")
}

fn show_for_exit_codes(success_symbol: &str, error_symbol: &str) -> String {
    format!("%(?.{success_symbol}.{error_symbol})")
}

fn show_when_job(symbol: &str) -> String {
    format!("%(1j.{symbol}.)")
}

fn write_top_separator(total_columns: u16) {
    for column in 0..total_columns {
        print!(
            "{}",
            if column % 2 == 0 {
                color_symbol("≥", Some(Color::Yellow))
            } else {
                color_symbol("v", Some(Color::Red))
            }
        );
    }
    print!("{}", color_symbol(":«(", Some(Color::Yellow)));
}

fn write_middle_separator(total_columns: u16, sections_length: u16) {
    print!("{}", color_symbol(")»:", Some(Color::Yellow)));
    for column in 0..total_columns.saturating_sub(sections_length) {
        print!(
            "{}",
            if column % 2 == 0 {
                color_symbol("-", Some(Color::Red))
            } else {
                color_symbol("=", Some(Color::Yellow))
            }
        );
    }
}

fn write_ip_section(ip: Option<&IpAddr>, sections_length: &mut u16) {
    let ip = match ip {
        Some(ip) => ip.to_string(),
        None => String::from("No Address Found"),
    };
    print!("{} {}", color_symbol(" ", Some(Color::Blue)), ip);
    *sections_length += ip.len() as u16;
}

fn write_disk_section(usage: &DiskUsage, sections_length: &mut u16) {
    print!(
        "  {}{}{}",
        color_symbol(
            "󰋊 ",
            match usage.status {
                DiskUsageStatus::Low => Some(Color::Green),
                DiskUsageStatus::Moderate => Some(Color::Yellow),
                DiskUsageStatus::High => Some(Color::Red),
            }
        ),
        usage.percentage,
        ZSH_PERCENTAGE_SYMBOL
    );
    *sections_length += calculate_number_length(usage.percentage as u16);
}

fn write_battery_section(charge: Option<&Charge>, sections: &mut u16) {
    if charge.is_none() {
        return;
    }
    let charge = charge.unwrap();
    print!(
        "  {} {}{}",
        match charge.status {
            BatteryChargeStatus::Critical =>
                color_symbol(if charge.is_charging { "󰢟" } else { "󰂎" }, Some(Color::Red)),
            BatteryChargeStatus::Low =>
                color_symbol(if charge.is_charging { "󱊤" } else { "󱊡" }, Some(Color::Red)),
            BatteryChargeStatus::Moderate => color_symbol(
                if charge.is_charging { "󱊥" } else { "󱊢" },
                Some(Color::Yellow)
            ),
            BatteryChargeStatus::High => color_symbol(
                if charge.is_charging { "󱊦" } else { "󱊣" },
                Some(Color::Green)
            ),
        },
        charge.percentage,
        ZSH_PERCENTAGE_SYMBOL
    );
    *sections += calculate_number_length(charge.percentage as u16) + 5;
}

fn write_calendar_section(time: &DateTime<Local>) {
    print!(
        "  {}{}{}",
        color_symbol("󰃭 ", Some(Color::Red)),
        time.format("(%a) %b %d"),
        day_ordinal(time)
    );
}

fn write_clock_section(time: &DateTime<Local>) {
    print!(
        "  {}{}",
        match DayFraction::from(time) {
            DayFraction::Dawn => color_symbol("󰭎 ", Some(Color::Cyan)),
            DayFraction::Morning => color_symbol("󰖨 ", Some(Color::Red)),
            DayFraction::Afternoon => color_symbol(" ", Some(Color::Blue)),
            DayFraction::Night => color_symbol("󰽥 ", Some(Color::Yellow)),
        },
        time.format("%Hh%Mm")
    );
}

fn write_root_section() {
    print!(
        "{}",
        show_when_root(&format!(
            "{}{}{}",
            color_symbol("{", Some(Color::Yellow)),
            color_symbol("#", Some(Color::Red)),
            color_symbol("}", Some(Color::Yellow))
        ))
    );
}

fn write_exit_code_section() {
    print!(
        "{}{}{}",
        color_symbol("{", Some(Color::Yellow)),
        show_for_exit_codes(
            &color_symbol(ZSH_EXIT_CODE, Some(Color::Yellow)),
            &color_symbol(ZSH_EXIT_CODE, Some(Color::Red))
        ),
        color_symbol("}⤐ ", Some(Color::Yellow))
    );
}

fn write_venv_section(venv: Option<&String>) {
    if let Some(venv) = venv {
        print!(
            " ({})",
            Path::new(venv).file_name().unwrap().to_string_lossy()
        );
    }
}

fn write_path_section(current_directory: &Path, repository: Option<&Repository>) {
    print!(" ");
    if repository.is_some() && !is_fs_root(&repository.unwrap().path) {
        print!(
            "{}",
            color_symbol(
                &format!(
                    "@/{}",
                    current_directory
                        .strip_prefix(repository.unwrap().path.parent().unwrap())
                        .unwrap()
                        .display()
                ),
                Some(Color::Red)
            )
        );
    } else {
        print!("{}", color_symbol("%~", Some(Color::Red)));
    }
}

fn write_git_section(repository: Option<&Repository>) {
    if repository.is_none() {
        return;
    }
    let repository = repository.unwrap();
    print!("{}", color_symbol(":«(", Some(Color::Yellow)));
    if let GitReference::RebaseHash(_) = repository.reference {
        print!("{}:", color_symbol("@rebase", Some(Color::Magenta)));
    }
    print!(
        "{}{}",
        match &repository.reference {
            GitReference::Branch(branch) => branch,
            GitReference::RebaseHash(hash) => hash,
        },
        color_symbol(")»", Some(Color::Yellow))
    );
    if repository.is_dirty {
        print!(" {}", color_symbol("✗ ", Some(Color::Cyan)));
    }
}

fn write_directory_access_section() {
    if !can_access_current_directory() {
        print!(" {}", color_symbol("", Some(Color::Cyan)));
    }
}

fn write_left_prompt() -> Result<(), Error> {
    let total_columns = terminal_size()
        .map(|(total_columns, _)| total_columns)
        .map_err(|_| Error::new("can not retrieve the terminal dimensions."))?;
    let ip = local_ip().ok();
    let disk_usage = disk_usage()?;
    let battery_charge = battery_charge()?;
    let time = Local::now();
    let current_directory = current_directory()?;
    let venv = environment_variable("VIRTUAL_ENV").ok();
    let repository = git_repository();
    let mut sections_length = LEFT_PROMPT_SECTIONS_CONSTANT_LENGTH;
    write_top_separator(total_columns);
    write_ip_section(ip.as_ref(), &mut sections_length);
    write_disk_section(&disk_usage, &mut sections_length);
    write_battery_section(battery_charge.as_ref(), &mut sections_length);
    write_calendar_section(&time);
    write_clock_section(&time);
    write_middle_separator(total_columns, sections_length);
    write_root_section();
    write_exit_code_section();
    write_venv_section(venv.as_ref());
    write_path_section(&current_directory, repository.as_ref());
    write_git_section(repository.as_ref());
    write_directory_access_section();
    println!(" ");
    Ok(())
}

fn write_directory_entry_total(total: usize, symbol: &str, color: Option<Color>) {
    if total > 0 {
        print!(
            " {}{}",
            color_symbol(symbol, color),
            total.to_formatted_string(&Locale::en)
        );
    }
}

fn write_directory_entries_section(entries_count: &DirectoryEntriesCount) {
    write_directory_entry_total(entries_count.total_directories, " ", Some(Color::Yellow));
    write_directory_entry_total(entries_count.total_files, " ", None);
    write_directory_entry_total(entries_count.total_sockets, "󱄙 ", Some(Color::Cyan));
    write_directory_entry_total(entries_count.total_fifos, "󰟦 ", Some(Color::Blue));
    write_directory_entry_total(
        entries_count.total_block_devices,
        "󰇖 ",
        Some(Color::Magenta),
    );
    write_directory_entry_total(
        entries_count.total_character_devices,
        "󱣴 ",
        Some(Color::Green),
    );
    write_directory_entry_total(entries_count.total_symlinks, "󰌷 ", Some(Color::Blue));
    write_directory_entry_total(entries_count.total_hidden_entries, "󰈉 ", Some(Color::Red));
    write_directory_entry_total(
        entries_count.total_temporary_entries,
        "󱣹 ",
        Some(Color::Magenta),
    );
}

fn write_jobs_section() {
    print!(
        "{}",
        show_when_job(&format!(
            " {} {}",
            color_symbol("", Some(Color::Magenta)),
            ZSH_TOTAL_JOBS
        ))
    );
}

fn write_right_prompt() -> Result<(), Error> {
    let entries_count = directory_entries_count()?;
    write_directory_entries_section(&entries_count);
    write_jobs_section();
    println!();
    Ok(())
}

fn main() -> ExitCode {
    match parse_arguments() {
        Ok(parse_type) => match parse_type {
            ArgumentParseType::Flag(Flag::MainHelp) => write_help(),
            ArgumentParseType::Flag(Flag::InitCommandHelp) => write_init_command_help(),
            ArgumentParseType::Flag(Flag::PromptCommandHelp) => write_prompt_command_help(),
            ArgumentParseType::Flag(Flag::Version) => write_version(),
            ArgumentParseType::Flag(Flag::Repository) => {
                if let Err(error) = open_repository() {
                    write_error(&error);
                    return ExitCode::FAILURE;
                }
            }
            ArgumentParseType::Command(Command::Init) => init_prompt(),
            ArgumentParseType::Command(Command::Prompt(side)) => {
                if let Err(error) = match side {
                    PromptSide::Left => write_left_prompt(),
                    PromptSide::Right => write_right_prompt(),
                } {
                    write_error(&error);
                    return ExitCode::FAILURE;
                }
            }
        },
        Err(error) => {
            write_error(&error);
            return ExitCode::FAILURE;
        }
    }
    ExitCode::SUCCESS
}
