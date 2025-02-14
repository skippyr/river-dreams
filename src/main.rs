use battery::{units::ratio::ratio, Manager as BatteryManager, State as BatteryState};
use chrono::{DateTime, Datelike, Local as LocalTime, Timelike};
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
use local_ip_address::local_ip as ip_address;
use num_format::{Locale, ToFormattedString};
use open::that as open_at_workspace;
#[cfg(target_os = "macos")]
use std::mem::zeroed;
use std::{
    env::{args as arguments, current_dir as std_current_directory, var as environment_variable},
    ffi::c_char,
    fs::read_to_string as read_file,
    net::IpAddr,
    path::{Path, PathBuf},
    process::exit as exit_process,
};

const NAME: &str = env!("CARGO_PKG_NAME");
const VERSION: &str = concat!("v", env!("CARGO_PKG_VERSION"));
const REPOSITORY: &str = env!("CARGO_PKG_HOMEPAGE");
const CREATION_YEAR: i32 = 2023;
const LICENSE_NAME: &str = concat!(env!("CARGO_PKG_LICENSE"), " License");
const LICENSE_TEXT: &str = include_str!("../LICENSE");
const AUTHOR_NAME: &str = "Sherman Rofeman";
const AUTHOR_EMAIL: &str = "skippyr.developer@icloud.com";
const LEFT_PROMPT_SECTIONS_CONSTANT_LENGTH: u16 = 42;
const ZSH_PERCENTAGE_SYMBOL: &str = "%%";
const ZSH_EXIT_CODE: &str = "%?";
const ZSH_TOTAL_JOBS: &str = "%j";
const GIT_DEFAULT_BRANCH_NAME: &str = "master";

enum Command {
    Init,
    Prompt,
}

impl Command {
    fn name(&self) -> &'static str {
        match self {
            Self::Init => "init",
            Self::Prompt => "prompt",
        }
    }
}

enum PromptSide {
    Left,
    Right,
}

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

enum DayFraction {
    Dawn,
    Morning,
    Afternoon,
    Night,
}

impl From<&DateTime<LocalTime>> for DayFraction {
    fn from(time: &DateTime<LocalTime>) -> Self {
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

enum EntryType {
    File,
    Directory,
    Socket,
    Fifo,
    Block,
    Character,
    Symlink,
}

struct DiskUsage {
    percentage: u8,
    status: DiskUsageStatus,
}

struct BatteryCharge {
    percentage: u8,
    status: BatteryChargeStatus,
    is_charging: bool,
}

struct GitRepository {
    path: PathBuf,
    reference: GitReference,
    is_dirty: bool,
}

#[derive(Default)]
struct DirectoryReport {
    total_files: usize,
    total_directories: usize,
    total_sockets: usize,
    total_fifos: usize,
    total_blocks: usize,
    total_characters: usize,
    total_symlinks: usize,
    total_hiddens: usize,
    total_temporaries: usize,
}

fn throw_error<T>(message: T) -> !
where
    T: Into<String>,
{
    eprintln!(
        "{}{}{} {} {}{} {}",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        NAME.dark_magenta().bold(),
        "(exit 1)".dark_yellow().bold(),
        ":".dark_magenta().bold(),
        message.into()
    );
    eprintln!(
        "{} use {} or {} for help instructions.",
        " INFO:".dark_cyan().bold(),
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
    exit_process(1);
}

fn write_help() {
    println!(
        "{}{}{} {} {NAME} <{}> [{}]...",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
        "COMMAND".dark_yellow().underlined(),
        "FLAGS".dark_cyan().underlined()
    );
    println!("Performs tasks related to the River Dreams theme.");
    println!();
    println!("{}", "❡ Available Commands".dark_magenta().bold());
    println!("    {}    initiates the prompt.", "init".dark_yellow());
    println!("    {}  writes a prompt side.", "prompt".dark_yellow());
    println!();
    println!(
        "{} use {} or {} with each for help instructions.",
        " INFO:".dark_cyan().bold(),
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
    println!();
    println!("{}", "❡ Available Flags".dark_magenta().bold());
    println!(
        "    {}, {}        shows the software help instructions.",
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
    println!(
        "    {}, {}     shows the software version.",
        "-v".dark_cyan(),
        "--version".dark_cyan()
    );
    println!(
        "    {}, {}     shows the software license text.",
        "-l".dark_cyan(),
        "--license".dark_cyan()
    );
    println!(
        "    {}, {}  opens the software repository.",
        "-g".dark_cyan(),
        "--repository".dark_cyan()
    );
    println!(
        "    {}, {}       drafts an e-mail to the software developer.",
        "-m".dark_cyan(),
        "--email".dark_cyan()
    );
}

fn write_init_help() {
    println!(
        "{}{}{} {} {NAME} init [{}]...",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
        "FLAGS".dark_cyan().underlined()
    );
    println!("Initiates the prompt.");
    println!();
    println!("It should be evaluated within ~/.zshrc:");
    println!("    echo 'eval $(river_dreams init)' >> ~/.zshrc;");
    println!();
    println!("{}", "❡ Available Flags".dark_magenta().bold());
    println!(
        "    {}, {}  shows the command help instructions.",
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
}

fn write_prompt_help() {
    println!(
        "{}{}{} {} {NAME} prompt <{}> [{}]...",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
        "SIDE".dark_yellow().underlined(),
        "FLAGS".dark_cyan().underlined()
    );
    println!("Writes a prompt side.");
    println!();
    println!("{}", "❡ Available Sides".dark_magenta().bold());
    println!("    {}   refers to the left prompt.", "left".dark_yellow());
    println!("    {}  refers to the right prompt.", "right".dark_yellow());
    println!();
    println!("{}", "❡ Available Flags".dark_magenta().bold());
    println!(
        "    {}, {}  shows the command help instructions.",
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
}

fn write_version() {
    println!( "{} {VERSION}", NAME.dark_magenta().bold());
    println!("Available at: {}.", REPOSITORY.dark_cyan().underlined());
    println!();
    println!("Licensed under the {LICENSE_NAME}.");
    println!(
        "Copyright © {CREATION_YEAR} {AUTHOR_NAME} <{}>",
        AUTHOR_EMAIL.dark_cyan().underlined()
    );
}

fn open_repository() {
    if let Err(_) = open_at_workspace(REPOSITORY) {
        throw_error("can not open the default web browser.");
    }
    println!("Opening the repository in the default web browser.");
}

fn draft_email() {
    if let Err(_) = open_at_workspace(format!("mailto:{AUTHOR_EMAIL}")) {
        throw_error("can not open the default mail client.");
    }
    println!("Drafting an e-mail in the default e-mail client.");
}

fn is_flag(argument: &str) -> bool {
    return argument.starts_with("-") || argument.starts_with("--");
}

fn init_prompt() {
    println!("setopt promptsubst;");
    println!("export VIRTUAL_ENV_DISABLE_PROMPT=1;");
    println!("PROMPT='$(river_dreams prompt left)';");
    println!("RPROMPT='$(river_dreams prompt right)';");
}

fn color_symbol(symbol: &str, color: Color) -> String {
    format!("%F{{{}}}{symbol}%f", color.ansi())
}

fn total_columns() -> u16 {
    match terminal_size().map(|(total_columns, _)| total_columns) {
        Ok(total_columns) => total_columns,
        Err(_) => throw_error("can not retrieve the terminal dimensions."),
    }
}

fn write_top_separator(total_columns: u16) {
    for column in 0..total_columns {
        print!(
            "{}",
            if column % 2 == 0 {
                color_symbol("≥", Color::Yellow)
            } else {
                color_symbol("v", Color::Red)
            }
        );
    }
    print!("{}", color_symbol(":«(", Color::Yellow));
}

fn write_ip_section(ip: Option<&IpAddr>, sections_length: &mut u16) {
    let ip = match ip {
        Some(ip) => ip.to_string(),
        None => String::from("No Address Found"),
    };
    print!("{} {}", color_symbol(" ", Color::Blue), ip);
    *sections_length += ip.len() as u16;
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

fn disk_usage() -> DiskUsage {
    let mut info = unsafe { std::mem::zeroed() };
    if unsafe { statvfs([b'/' as c_char, 0].as_ptr(), &mut info) } < 0 {
        throw_error("can not retrieve the disk information.");
    }
    let total_bytes = info.f_frsize * info.f_blocks as u64;
    let available_bytes = info.f_frsize * info.f_bavail as u64;
    let free_bytes = total_bytes - available_bytes;
    let percentage = (free_bytes as f64 / total_bytes as f64 * 100.0) as u8;
    DiskUsage {
        percentage,
        status: DiskUsageStatus::from(percentage),
    }
}

fn write_disk_section(usage: &DiskUsage, sections_length: &mut u16) {
    print!(
        "  {}{}{}",
        color_symbol(
            "󰋊 ",
            match usage.status {
                DiskUsageStatus::Low => Color::Green,
                DiskUsageStatus::Moderate => Color::Yellow,
                DiskUsageStatus::High => Color::Red,
            }
        ),
        usage.percentage,
        ZSH_PERCENTAGE_SYMBOL
    );
    *sections_length += calculate_number_length(usage.percentage as u16);
}

fn battery_charge() -> Option<BatteryCharge> {
    const SUPPLY_ERROR: &str = "can not retrieve info about the energy supply of the computer.";
    let manager = match BatteryManager::new() {
        Ok(manager) => manager,
        Err(_) => throw_error(SUPPLY_ERROR),
    };
    for battery in match manager.batteries() {
        Ok(batteries) => batteries,
        Err(_) => throw_error(SUPPLY_ERROR),
    } {
        let battery = match battery {
            Ok(battery) => battery,
            Err(_) => continue,
        };
        let percentage = (battery.state_of_charge().get::<ratio>() * 100.0).round() as u8;
        return Some(BatteryCharge {
            percentage,
            status: BatteryChargeStatus::from(percentage),
            is_charging: matches!(
                battery.state(),
                BatteryState::Charging | BatteryState::Full | BatteryState::Unknown
            ),
        });
    }
    None
}

fn write_battery_section(charge: Option<&BatteryCharge>, sections_length: &mut u16) {
    if charge.is_none() {
        return;
    }
    let charge = charge.unwrap();
    print!(
        "  {} {}{}",
        match charge.status {
            BatteryChargeStatus::Critical =>
                color_symbol(if charge.is_charging { "󰢟" } else { "󰂎" }, Color::Red),
            BatteryChargeStatus::Low =>
                color_symbol(if charge.is_charging { "󱊤" } else { "󱊡" }, Color::Red),
            BatteryChargeStatus::Moderate =>
                color_symbol(if charge.is_charging { "󱊥" } else { "󱊢" }, Color::Yellow),
            BatteryChargeStatus::High =>
                color_symbol(if charge.is_charging { "󱊦" } else { "󱊣" }, Color::Green),
        },
        charge.percentage,
        ZSH_PERCENTAGE_SYMBOL
    );
    *sections_length += calculate_number_length(charge.percentage as u16) + 5;
}

fn is_ordinal(time: &DateTime<LocalTime>, ordinal: u32) -> bool {
    (time.day() - ordinal) % 10 == 0
}

fn day_ordinal(time: &DateTime<LocalTime>) -> &'static str {
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

fn write_calendar_section(time: &DateTime<LocalTime>) {
    print!(
        "  {}{}{}",
        color_symbol("󰃭 ", Color::Red),
        time.format("(%a) %b %d"),
        day_ordinal(time)
    );
}

fn write_clock_section(time: &DateTime<LocalTime>) {
    print!(
        "  {}{}",
        match DayFraction::from(time) {
            DayFraction::Dawn => color_symbol("󰭎 ", Color::Cyan),
            DayFraction::Morning => color_symbol("󰖨 ", Color::Red),
            DayFraction::Afternoon => color_symbol(" ", Color::Blue),
            DayFraction::Night => color_symbol("󰽥 ", Color::Yellow),
        },
        time.format("%Hh%Mm")
    );
}

fn write_middle_separator(total_columns: u16, sections_length: u16) {
    print!("{}", color_symbol(")»:", Color::Yellow));
    for column in 0..total_columns.saturating_sub(sections_length) {
        print!(
            "{}",
            if column % 2 == 0 {
                color_symbol("-", Color::Red)
            } else {
                color_symbol("=", Color::Yellow)
            }
        );
    }
}

fn show_when_root(symbol: &str) -> String {
    format!("%(#.{symbol}.)")
}

fn write_root_section() {
    print!(
        "{}",
        show_when_root(&format!(
            "{}{}{}",
            color_symbol("{", Color::Yellow),
            color_symbol("#", Color::Red),
            color_symbol("}", Color::Yellow)
        ))
    );
}

fn show_in_exit_codes(success_symbol: &str, error_symbol: &str) -> String {
    format!("%(?.{success_symbol}.{error_symbol})")
}

fn write_exit_code_section() {
    print!(
        "{}{}{}",
        color_symbol("{", Color::Yellow),
        show_in_exit_codes(
            &color_symbol(ZSH_EXIT_CODE, Color::Yellow),
            &color_symbol(ZSH_EXIT_CODE, Color::Red)
        ),
        color_symbol("}⤐ ", Color::Yellow)
    );
}

fn write_virtual_env_section() {
    if let Some(virtual_env) = environment_variable("VIRTUAL_ENV").ok() {
        print!(
            " ({})",
            Path::new(&virtual_env)
                .file_name()
                .unwrap()
                .to_string_lossy()
        );
    }
}

fn current_directory() -> PathBuf {
    if let Ok(pwd) = std_current_directory() {
        return pwd;
    } else if let Ok(pwd) = environment_variable("PWD") {
        return PathBuf::from(pwd);
    }
    throw_error("can not resolve the current directory.");
}

fn git_repository() -> Option<GitRepository> {
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
    } else if let Ok(head_data) = read_file(repository.path().join("HEAD")) {
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
    Some(GitRepository {
        path: if repository.is_bare() {
            repository.path().to_path_buf()
        } else {
            repository.workdir().unwrap().to_path_buf()
        },
        reference: match reference {
            Some(reference) => reference,
            None => GitReference::Branch(String::from(GIT_DEFAULT_BRANCH_NAME)),
        },
        is_dirty,
    })
}

fn is_file_system_root(path: &Path) -> bool {
    path.ancestors().count() == 1
}

fn write_path_section(current_directory: &Path, repository: Option<&GitRepository>) {
    print!(" ");
    if repository.is_some() && !is_file_system_root(&repository.unwrap().path) {
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
                Color::Red
            )
        );
    } else {
        print!("{}", color_symbol("%~", Color::Red));
    }
}

fn write_git_section(repository: Option<&GitRepository>) {
    if repository.is_none() {
        return;
    }
    let repository = repository.unwrap();
    print!("{}", color_symbol(":«(", Color::Yellow));
    if let GitReference::RebaseHash(_) = repository.reference {
        print!("{}:", color_symbol("@rebase", Color::Magenta));
    }
    print!(
        "{}{}",
        match &repository.reference {
            GitReference::Branch(branch) => branch,
            GitReference::RebaseHash(hash) => hash,
        },
        color_symbol(")»", Color::Yellow)
    );
    if repository.is_dirty {
        print!(" {}", color_symbol("✗", Color::Cyan));
    }
}

fn can_access_current_directory() -> bool {
    unsafe { access([b'.' as c_char, 0].as_ptr(), W_OK) == 0 }
}

fn write_directory_access_section() {
    if !can_access_current_directory() {
        print!(" {}", color_symbol("", Color::Cyan));
    }
}

fn write_left_prompt() {
    let total_columns = total_columns();
    let mut sections_length: u16 = LEFT_PROMPT_SECTIONS_CONSTANT_LENGTH;
    let disk_usage = disk_usage();
    let battery_charge = battery_charge();
    let local_time = LocalTime::now();
    let current_directory = current_directory();
    let repository = git_repository();
    write_top_separator(total_columns);
    write_ip_section(ip_address().ok().as_ref(), &mut sections_length);
    write_disk_section(&disk_usage, &mut sections_length);
    write_battery_section(battery_charge.as_ref(), &mut sections_length);
    write_calendar_section(&local_time);
    write_clock_section(&local_time);
    write_middle_separator(total_columns, sections_length);
    write_root_section();
    write_exit_code_section();
    write_virtual_env_section();
    write_path_section(&current_directory, repository.as_ref());
    write_git_section(repository.as_ref());
    write_directory_access_section();
    println!(" ");
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

#[cfg(target_os = "macos")]
fn entry_lstat(entry: &dirent) -> Result<stat, ()> {
    let mut entry_stat = unsafe { zeroed() };
    if unsafe { lstat(entry.d_name.as_ptr(), &mut entry_stat) } < 0 {
        Err(())
    } else {
        Ok(entry_stat)
    }
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

fn entry_type(entry: &dirent) -> EntryType {
    match entry.d_type {
        DT_DIR => EntryType::Directory,
        DT_SOCK => EntryType::Socket,
        DT_FIFO => EntryType::Fifo,
        DT_BLK => EntryType::Block,
        DT_CHR => EntryType::Character,
        DT_LNK => EntryType::Symlink,
        _ => EntryType::File,
    }
}

fn directory_report() -> DirectoryReport {
    let stream = unsafe { opendir([b'.' as c_char, 0].as_ptr()) };
    let mut report = DirectoryReport::default();
    if stream.is_null() {
        return report;
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
            report.total_temporaries += 1;
        }
        match is_hidden_entry(&entry) {
            Ok(is_hidden) => {
                if is_hidden {
                    report.total_hiddens += 1;
                }
            }
            Err(_) => continue,
        }
        match entry_type(&entry) {
            EntryType::Directory => report.total_directories += 1,
            EntryType::Socket => report.total_sockets += 1,
            EntryType::Fifo => report.total_fifos += 1,
            EntryType::Block => report.total_blocks += 1,
            EntryType::Character => report.total_characters += 1,
            EntryType::Symlink => report.total_symlinks += 1,
            EntryType::File => report.total_files += 1,
        }
    }
    unsafe {
        closedir(stream);
    }
    report
}

fn write_directory_report_field(total: usize, symbol: &str, color: Option<Color>) {
    if total > 0 {
        print!(
            " {}{}",
            if color.is_none() {
                String::from(symbol)
            } else {
                color_symbol(symbol, color.unwrap())
            },
            total.to_formatted_string(&Locale::en)
        );
    }
}

fn write_directory_entries_section(report: &DirectoryReport) {
    write_directory_report_field(report.total_directories, " ", Some(Color::Yellow));
    write_directory_report_field(report.total_files, " ", None);
    write_directory_report_field(report.total_sockets, "󱄙 ", Some(Color::Cyan));
    write_directory_report_field(report.total_fifos, "󰟦 ", Some(Color::Blue));
    write_directory_report_field(report.total_blocks, "󰇖 ", Some(Color::Magenta));
    write_directory_report_field(report.total_characters, "󱣴 ", Some(Color::Green));
    write_directory_report_field(report.total_symlinks, "󰌷 ", Some(Color::Blue));
    write_directory_report_field(report.total_hiddens, "󰈉 ", Some(Color::Red));
    write_directory_report_field(report.total_temporaries, "󱣹 ", Some(Color::Magenta));
}

fn show_when_job(symbol: &str) -> String {
    format!("%(1j.{symbol}.)")
}

fn write_jobs_section() {
    print!(
        "{}",
        show_when_job(&format!(
            " {} {}",
            color_symbol("", Color::Magenta),
            ZSH_TOTAL_JOBS
        ))
    );
}

fn write_right_prompt() {
    let directory_report = directory_report();
    write_directory_entries_section(&directory_report);
    write_jobs_section();
    println!();
}

fn main() {
    let arguments = arguments().skip(1).collect::<Vec<_>>();
    let mut active_command: Option<Command> = None;
    if let Some(command_argument) = arguments.first() {
        if command_argument == "init" {
            active_command = Some(Command::Init);
        } else if command_argument == "prompt" {
            active_command = Some(Command::Prompt)
        } else if !is_flag(command_argument) {
            throw_error(format!(
                "invalid command \"{}\" provided.",
                command_argument
            ));
        }
    }
    for argument in &arguments {
        if argument == "-h" || argument == "--help" {
            match active_command {
                None => write_help(),
                Some(Command::Prompt) => write_prompt_help(),
                Some(Command::Init) => write_init_help(),
            }
            return;
        }
        if active_command.is_none() {
            if argument == "-v" || argument == "--version" {
                write_version();
                return;
            } else if argument == "-g" || argument == "--repository" {
                open_repository();
                return;
            } else if argument == "-m" || argument == "--email" {
                draft_email();
                return;
            } else if argument == "-l" || argument == "--license" {
                println!("{LICENSE_TEXT}");
                return;
            }
        }
        if is_flag(argument) {
            if let Some(active_command) = active_command {
                throw_error(format!(
                    "invalid flag \"{}\" provided for \"{}\" command.",
                    argument,
                    active_command.name()
                ));
            } else {
                throw_error(format!("invalid flag \"{}\" provided.", argument));
            }
        }
    }
    match active_command {
        None => throw_error("no command provided."),
        Some(Command::Init) => init_prompt(),
        Some(Command::Prompt) => {
            let mut prompt_side = PromptSide::Left;
            if arguments.len() == 1 {
                throw_error("no prompt side provided.");
            } else if arguments[1] == "right" {
                prompt_side = PromptSide::Right;
            } else if arguments[1] != "left" {
                throw_error(format!(
                    "invalid prompt side \"{}\" provided.",
                    arguments[1]
                ));
            }
            match prompt_side {
                PromptSide::Left => write_left_prompt(),
                PromptSide::Right => write_right_prompt(),
            }
        }
    }
}
