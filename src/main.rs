use battery::{units::ratio::ratio, Manager as BatMgr, State as BatState};
use chrono::{DateTime, Datelike, Local, Timelike};
use crossterm::{style::Stylize, terminal::size as term_size};
use git2::{Repository as Git2Repo, RepositoryState as Git2State, Status as Git2Status};
use libc::{
    access, closedir, dirent, opendir, readdir, statvfs, strlen, DT_BLK, DT_CHR, DT_DIR, DT_FIFO,
    DT_LNK, DT_SOCK, W_OK,
};
#[cfg(target_os = "macos")]
use libc::{lstat, stat, UF_HIDDEN};
use local_ip_address::local_ip;
use num_format::{Locale, ToFormattedString};
use open::that as open_in_browser;
#[cfg(target_os = "macos")]
use std::mem::zeroed;
use std::{
    env::{args, current_dir, var as env_var},
    ffi::c_char,
    fmt::{Display, Error as FmtError, Formatter},
    fs::read_to_string,
    net::IpAddr,
    path::{Path, PathBuf},
    process::ExitCode,
};

const NAME: &str = env!("CARGO_PKG_NAME");
const VERSION: &str = env!("CARGO_PKG_VERSION");
const REPO: &str = env!("CARGO_PKG_HOMEPAGE");
const LICENSE: &str = env!("CARGO_PKG_LICENSE");
const AUTHOR_NAME: &str = "Sherman Rofeman";
const AUTHOR_EMAIL: &str = "skippyr.developer@icloud.com";
const YEAR: u16 = 2023;
const OS_NAME: &str = if cfg!(target_os = "macos") {
    "macOS"
} else {
    "Linux"
};
const CPU_ARCH: &str = if cfg!(target_arch = "aarch64") {
    "aarch64"
} else {
    "x86_64"
};
const SEC_CONST_LEN: u16 = 42;
const DFT_BRANCH: &str = "master";
const PERC_SYM: &str = "%%";
const EXIT_CODE: &str = "%?";
const JOBS: &str = "%j";

enum ValOpt<T> {
    Some(T),
    None,
    Invalid,
}

#[derive(Clone, Copy)]
enum Opt {
    Help,
    InitCmdHelp,
    PromptCmdHelp,
    Version,
    Repo,
}

enum Cmd {
    Init,
    Prompt(Side),
}

enum ArgParse {
    Opt(Opt),
    Cmd(Cmd),
}

#[derive(Default, Clone, Copy)]
enum Side {
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
enum UseStatus {
    Low,
    Moderate,
    High,
}

impl From<u8> for UseStatus {
    fn from(perc: u8) -> Self {
        match perc {
            0..60 => Self::Low,
            60..80 => Self::Moderate,
            _ => Self::High,
        }
    }
}

#[derive(Clone, Copy)]
enum ChargeStatus {
    Critical,
    Low,
    Moderate,
    High,
}

impl ChargeStatus {
    fn from(perc: u8) -> Self {
        match perc {
            0..5 => Self::Critical,
            5..30 => Self::Low,
            30..60 => Self::Moderate,
            _ => Self::High,
        }
    }
}

#[derive(Clone, Copy)]
enum DayFrac {
    Dawn,
    Morning,
    Afternoon,
    Night,
}

impl From<&DateTime<Local>> for DayFrac {
    fn from(time: &DateTime<Local>) -> Self {
        match time.hour() {
            0..6 => Self::Dawn,
            6..12 => Self::Morning,
            12..18 => Self::Afternoon,
            _ => Self::Night,
        }
    }
}

enum Ref {
    Branch(String),
    RebHash(String),
}

#[derive(Clone, Copy)]
enum EntType {
    Dir,
    File,
    Socket,
    Fifo,
    Blk,
    Char,
    Symlink,
}

struct Use {
    perc: u8,
    status: UseStatus,
}

struct Charge {
    perc: u8,
    status: ChargeStatus,
    is_charg: bool,
}

struct Repo {
    path: PathBuf,
    r#ref: Ref,
    is_dirty: bool,
}

#[derive(Default)]
struct DirEnts {
    dirs: usize,
    files: usize,
    sockets: usize,
    fifos: usize,
    blks: usize,
    chars: usize,
    symlinks: usize,
    hiddens: usize,
    tmps: usize,
}

#[derive(Debug)]
struct Error(String);

impl Error {
    fn new<T: ToString>(msg: T) -> Self {
        Self(msg.to_string())
    }
}

impl Display for Error {
    fn fmt(&self, fmt: &mut Formatter) -> Result<(), FmtError> {
        write!(fmt, "{}", self.0)
    }
}

fn is_opt(arg: &str) -> bool {
    let c = arg.chars().collect::<Vec<_>>();
    (c.len() == 2 && c[0] == '-' && c[1].is_ascii_lowercase())
        || (c.len() > 2 && c[0] == '-' && c[1] == '-' && c[2].is_ascii_lowercase())
}

fn is_ill_opt(arg: &str) -> bool {
    arg == "-" || arg == "--"
}

fn parse_args() -> Result<ArgParse, Error> {
    let args = args().skip(1).collect::<Vec<_>>();
    let mut cmd = ValOpt::None;
    if let Some(arg) = args.first() {
        if arg == "init" {
            cmd = ValOpt::Some(Cmd::Init);
        } else if arg == "prompt" {
            cmd = ValOpt::Some(Cmd::Prompt(Side::default()));
        } else {
            cmd = ValOpt::Invalid;
        }
    }
    for arg in &args {
        if arg == "-h" || arg == "--help" {
            match cmd {
                ValOpt::Some(Cmd::Init) => return Ok(ArgParse::Opt(Opt::InitCmdHelp)),
                ValOpt::Some(Cmd::Prompt(_)) => return Ok(ArgParse::Opt(Opt::PromptCmdHelp)),
                _ => return Ok(ArgParse::Opt(Opt::Help)),
            }
        } else if arg == "-v" || arg == "--version" {
            return Ok(ArgParse::Opt(Opt::Version));
        } else if arg == "-g" || arg == "--repo" {
            return Ok(ArgParse::Opt(Opt::Repo));
        } else if is_opt(arg) {
            return Err(Error::new(format_args!(
                r#"invalid option "{arg}" provided."#
            )));
        } else if is_ill_opt(arg) {
            return Err(Error::new(format_args!(r#"ill option "{arg}" provided."#)));
        }
    }
    match cmd {
        ValOpt::Some(Cmd::Init) => return Ok(ArgParse::Cmd(Cmd::Init)),
        ValOpt::None => return Err(Error::new("no command provided.")),
        ValOpt::Invalid => {
            return Err(Error::new(format_args!(
                r#"invalid command "{}" provided."#,
                args[0]
            )))
        }
        _ => {}
    }
    let mut side = Side::Left;
    if let Some(arg) = args.get(1) {
        if arg == "right" {
            side = Side::Right;
        } else if arg != "left" {
            return Err(Error::new(format_args!(
                r#"invalid prompt side "{arg}" provided."#
            )));
        }
    } else {
        return Err(Error::new("no prompt side provided."));
    }
    Ok(ArgParse::Cmd(Cmd::Prompt(side)))
}

fn open_repo() -> Result<(), Error> {
    open_in_browser(REPO).map_err(|_| Error::new("can not open the software repository."))
}

fn write_err(err: &Error) {
    eprintln!(
        "{}{}{} {} {}{} {err}",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        NAME.dark_magenta().bold(),
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

fn write_usage(cmd: Option<&str>, args: Option<&[&str]>, desc: &str) {
    print!(
        "{}{}{} {} {NAME}",
        ":".dark_yellow().bold(),
        "<>".dark_red().bold(),
        "::".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
    );
    if let Some(cmd) = cmd {
        print!(" {cmd}");
    }
    if let Some(args) = args {
        for arg in args {
            print!(" <{}>", arg.dark_yellow().underlined());
        }
    }
    println!(" [{}]...", "OPTIONS".dark_yellow().underlined());
    println!("{desc}");
}

fn arr_lpad<'a, T: AsRef<[&'a str]>>(items: &T) -> usize {
    let mut lpad = 0;
    for item in items.as_ref() {
        if item.len() > lpad {
            lpad = item.len();
        }
    }
    lpad
}

fn write_items(title: &str, items: &[[&str; 2]]) {
    write_title(title);
    let lpad = arr_lpad(&items.iter().map(|item| item[0]).collect::<Vec<_>>());
    for item in items {
        println!(
            "    {}  {}",
            format!("{:lpad$}", item[0]).dark_yellow(),
            item[1]
        );
    }
}

fn write_opts(opts: &[[&str; 3]]) {
    write_title("Available Options");
    let lpad = arr_lpad(&opts.iter().map(|opt| opt[1]).collect::<Vec<_>>());
    for opt in opts {
        println!(
            "    {}, {}  {}",
            format!("-{}", opt[0]).dark_cyan(),
            format!("--{:lpad$}", opt[1]).dark_cyan(),
            opt[2]
        );
    }
}

fn write_code(code: &str, lang: &str) {
    println!("    {}{}", "```".dark_grey(), lang.dark_cyan());
    println!("    {code}");
    println!("    {}", "```".dark_grey());
}

fn write_info(msg: &str, use_stdout: bool) {
    let msg = format!("{} {msg}", "  info:".dark_cyan().bold());
    if use_stdout {
        println!("{msg}");
    } else {
        eprintln!("{msg}");
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
    write_opts(&[
        ["h", "help", "shows the software help instructions."],
        ["v", "version", "shows the software version."],
        ["g", "repo", "opens the software repository."],
    ]);
}

fn write_init_cmd_help() {
    write_usage(Some("init"), None, "Initiates the prompt.");
    println!();
    println!("Use it within an eval call in ~/.zshrc:");
    write_code("eval $(river_dreams init);", "zsh");
    println!();
    write_opts(&[["h", "help", "shows the command help instructions."]]);
}

fn write_prompt_cmd_help() {
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
    write_opts(&[["h", "help", "shows the command instructions."]]);
}

fn write_version() {
    println!(
        "{} v{VERSION} {}",
        NAME.dark_magenta().bold(),
        format!("({OS_NAME} {CPU_ARCH})").dark_grey()
    );
    println!("Repo at: {}.", REPO.dark_cyan().underlined());
    println!();
    println!("{LICENSE} License");
    println!(
        "Copyright (c) {YEAR} {AUTHOR_NAME} <{}>",
        AUTHOR_EMAIL.dark_cyan().underlined()
    );
}

fn init_prompt() {
    println!("setopt promptsubst;");
    println!("export VIRTUAL_ENV_DISABLE_PROMPT=1;");
    println!("PROMPT='$(river_dreams prompt left)';");
    println!("RPROMPT='$(river_dreams prompt right)';");
}

fn num_len(mut num: u16) -> u16 {
    let mut len = if num == 0 { 1 } else { 0 };
    loop {
        if num == 0 {
            break;
        }
        len += 1;
        num /= 10;
    }
    len
}

fn disk_use() -> Result<Use, Error> {
    let mut info: statvfs = unsafe { std::mem::zeroed() };
    if unsafe { statvfs(['/' as c_char, 0].as_ptr(), &mut info) } < 0 {
        return Err(Error::new(
            "can not retrieve info about the main disk of the computer.",
        ));
    }
    let tot = info.f_frsize * info.f_blocks as u64;
    let avail = info.f_frsize * info.f_bavail as u64;
    let free = tot - avail;
    let perc = (free as f64 / tot as f64 * 100.0) as u8;
    Ok(Use {
        perc,
        status: UseStatus::from(perc),
    })
}

fn bat_charge() -> Result<Option<Charge>, Error> {
    const SUPPLY_ERR: &str = "can not retrieve info about the energy supply of the computer.";
    for bat in BatMgr::new()
        .map_err(|_| Error::new(SUPPLY_ERR))?
        .batteries()
        .map_err(|_| Error::new(SUPPLY_ERR))?
    {
        let bat = match bat {
            Ok(bat) => bat,
            Err(_) => continue,
        };
        let perc = (bat.state_of_charge().get::<ratio>() * 100.0).round() as u8;
        return Ok(Some(Charge {
            perc,
            status: ChargeStatus::from(perc),
            is_charg: matches!(
                bat.state(),
                BatState::Charging | BatState::Full | BatState::Unknown
            ),
        }));
    }
    Ok(None)
}

fn clr_sym(sym: &str, clr: Option<Color>) -> String {
    match clr {
        Some(clr) => format!("%F{{{}}}{sym}%f", clr.ansi()),
        None => sym.to_owned(),
    }
}

fn is_ord(time: &DateTime<Local>, ord: u32) -> bool {
    (time.day() - ord) % 10 == 0
}

fn day_ord(time: &DateTime<Local>) -> &'static str {
    if is_ord(time, 1) {
        "st"
    } else if is_ord(time, 2) {
        "nd"
    } else if is_ord(time, 3) {
        "rd"
    } else {
        "th"
    }
}

fn is_fs_root(path: &Path) -> bool {
    path.ancestors().count() == 1
}

fn pwd() -> Result<PathBuf, Error> {
    if let Ok(pwd) = current_dir() {
        return Ok(pwd);
    } else if let Ok(pwd) = env_var("PWD") {
        return Ok(PathBuf::from(pwd));
    }
    Err(Error::new("can not resolve the current directory."))
}

fn can_acc_pwd() -> bool {
    unsafe { access(['.' as c_char, 0].as_ptr(), W_OK) == 0 }
}

fn is_dft_ent(ent: &dirent) -> bool {
    (ent.d_name[0] == '.' as c_char && ent.d_name[1] == 0)
        || (ent.d_name[0] == '.' as c_char && ent.d_name[1] == '.' as c_char && ent.d_name[2] == 0)
}

fn is_tmp_ent(ent: &dirent) -> bool {
    ent.d_name[unsafe { strlen(ent.d_name.as_ptr()) } - 1] == '~' as c_char
}

fn is_hidden_ent(ent: &dirent) -> Result<bool, ()> {
    if ent.d_name[0] == '.' as c_char {
        return Ok(true);
    }
    if cfg!(target_os = "macos") {
        ent_lstat(ent).map(|stat| stat.st_flags & UF_HIDDEN != 0)
    } else {
        Ok(false)
    }
}

#[cfg(target_os = "macos")]
fn ent_lstat(ent: &dirent) -> Result<stat, ()> {
    let mut ent_stat = unsafe { zeroed() };
    if unsafe { lstat(ent.d_name.as_ptr(), &mut ent_stat) } < 0 {
        Err(())
    } else {
        Ok(ent_stat)
    }
}

fn ent_type(ent: &dirent) -> EntType {
    match ent.d_type {
        DT_DIR => EntType::Dir,
        DT_SOCK => EntType::Socket,
        DT_FIFO => EntType::Fifo,
        DT_BLK => EntType::Blk,
        DT_CHR => EntType::Char,
        DT_LNK => EntType::Symlink,
        _ => EntType::File,
    }
}

fn dir_ents() -> Result<DirEnts, Error> {
    let dir = unsafe { opendir(['.' as c_char, 0].as_ptr()) };
    let mut dir_ents = DirEnts::default();
    if dir.is_null() {
        return Err(Error::new("can not access the current directory."));
    }
    loop {
        let ent = unsafe { readdir(dir) };
        if ent.is_null() {
            break;
        }
        let ent = unsafe { *ent };
        if is_dft_ent(&ent) {
            continue;
        } else if is_tmp_ent(&ent) {
            dir_ents.tmps += 1;
        }
        match is_hidden_ent(&ent) {
            Ok(is_hidden) => {
                if is_hidden {
                    dir_ents.hiddens += 1;
                }
            }
            Err(_) => continue,
        }
        match ent_type(&ent) {
            EntType::Dir => dir_ents.dirs += 1,
            EntType::Socket => dir_ents.sockets += 1,
            EntType::Fifo => dir_ents.fifos += 1,
            EntType::Blk => dir_ents.blks += 1,
            EntType::Char => dir_ents.chars += 1,
            EntType::Symlink => dir_ents.symlinks += 1,
            EntType::File => dir_ents.files += 1,
        }
    }
    unsafe {
        closedir(dir);
    }
    Ok(dir_ents)
}

fn git_repo() -> Option<Repo> {
    let repo = match Git2Repo::discover(".") {
        Ok(repo) => repo,
        Err(_) => return None,
    };
    let mut r#ref = None;
    if let Ok(head) = repo.head() {
        if repo.state() == Git2State::RebaseInteractive {
            if let Some(hash) = head.target() {
                r#ref = Some(Ref::RebHash(
                    hash.to_string().chars().take(7).collect::<String>(),
                ));
            }
        } else if let Some(branch) = head.shorthand() {
            r#ref = Some(Ref::Branch(String::from(branch)));
        }
    } else if let Ok(head_data) = read_to_string(repo.path().join("HEAD")) {
        if let Some(branch) = head_data.trim().strip_prefix("ref: refs/heads/") {
            r#ref = Some(Ref::Branch(branch.to_string()));
        }
    } else if let Ok(cfg) = repo.config() {
        if let Ok(branch) = cfg.get_string("init.defaultBranch") {
            r#ref = Some(Ref::Branch(branch));
        }
    }
    let mut is_dirty: bool = false;
    if let Ok(ents) = repo.statuses(None) {
        for ent in &ents {
            if ent.status().contains(Git2Status::IGNORED) {
                continue;
            } else if ent.status().intersects(
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
    Some(Repo {
        path: if repo.is_bare() {
            repo.path().to_path_buf()
        } else {
            repo.workdir().unwrap().to_path_buf()
        },
        r#ref: match r#ref {
            Some(r#ref) => r#ref,
            None => Ref::Branch(String::from(DFT_BRANCH)),
        },
        is_dirty,
    })
}

fn show_when_root(sym: &str) -> String {
    format!("%(#.{sym}.)")
}

fn show_in_exit_codes(ok_sym: &str, err_sym: &str) -> String {
    format!("%(?.{ok_sym}.{err_sym})")
}

fn show_when_job(sym: &str) -> String {
    format!("%(1j.{sym}.)")
}

fn write_top_sep(cols: u16) {
    for col in 0..cols {
        print!(
            "{}",
            if col % 2 == 0 {
                clr_sym("≥", Some(Color::Yellow))
            } else {
                clr_sym("v", Some(Color::Red))
            }
        );
    }
    print!("{}", clr_sym(":«(", Some(Color::Yellow)));
}

fn write_mid_sep(cols: u16, sec_len: u16) {
    print!("{}", clr_sym(")»:", Some(Color::Yellow)));
    for col in 0..cols.saturating_sub(sec_len) {
        print!(
            "{}",
            if col % 2 == 0 {
                clr_sym("-", Some(Color::Red))
            } else {
                clr_sym("=", Some(Color::Yellow))
            }
        );
    }
}

fn write_ip_sec(ip: Option<&IpAddr>, sec_len: &mut u16) {
    let ip = match ip {
        Some(ip) => ip.to_string(),
        None => String::from("No Address Found"),
    };
    print!("{} {}", clr_sym(" ", Some(Color::Blue)), ip);
    *sec_len += ip.len() as u16;
}

fn write_disk_sec(r#use: &Use, sec_len: &mut u16) {
    print!(
        "  {}{}{}",
        clr_sym(
            "󰋊 ",
            match r#use.status {
                UseStatus::Low => Some(Color::Green),
                UseStatus::Moderate => Some(Color::Yellow),
                UseStatus::High => Some(Color::Red),
            }
        ),
        r#use.perc,
        PERC_SYM
    );
    *sec_len += num_len(r#use.perc as u16);
}

fn write_bat_sec(charge: Option<&Charge>, sec_len: &mut u16) {
    if charge.is_none() {
        return;
    }
    let charge = charge.unwrap();
    print!(
        "  {} {}{}",
        match charge.status {
            ChargeStatus::Critical =>
                clr_sym(if charge.is_charg { "󰢟" } else { "󰂎" }, Some(Color::Red)),
            ChargeStatus::Low => clr_sym(if charge.is_charg { "󱊤" } else { "󱊡" }, Some(Color::Red)),
            ChargeStatus::Moderate =>
                clr_sym(if charge.is_charg { "󱊥" } else { "󱊢" }, Some(Color::Yellow)),
            ChargeStatus::High =>
                clr_sym(if charge.is_charg { "󱊦" } else { "󱊣" }, Some(Color::Green)),
        },
        charge.perc,
        PERC_SYM
    );
    *sec_len += num_len(charge.perc as u16) + 5;
}

fn write_cal_sec(time: &DateTime<Local>) {
    print!(
        "  {}{}{}",
        clr_sym("󰃭 ", Some(Color::Red)),
        time.format("(%a) %b %d"),
        day_ord(time)
    );
}

fn write_clk_sec(time: &DateTime<Local>) {
    print!(
        "  {}{}",
        match DayFrac::from(time) {
            DayFrac::Dawn => clr_sym("󰭎 ", Some(Color::Cyan)),
            DayFrac::Morning => clr_sym("󰖨 ", Some(Color::Red)),
            DayFrac::Afternoon => clr_sym(" ", Some(Color::Blue)),
            DayFrac::Night => clr_sym("󰽥 ", Some(Color::Yellow)),
        },
        time.format("%Hh%Mm")
    );
}

fn write_root_sec() {
    print!(
        "{}",
        show_when_root(&format!(
            "{}{}{}",
            clr_sym("{", Some(Color::Yellow)),
            clr_sym("#", Some(Color::Red)),
            clr_sym("}", Some(Color::Yellow))
        ))
    );
}

fn write_exit_code_sec() {
    print!(
        "{}{}{}",
        clr_sym("{", Some(Color::Yellow)),
        show_in_exit_codes(
            &clr_sym(EXIT_CODE, Some(Color::Yellow)),
            &clr_sym(EXIT_CODE, Some(Color::Red))
        ),
        clr_sym("}⤐ ", Some(Color::Yellow))
    );
}

fn write_venv_sec(venv: Option<&String>) {
    if let Some(venv) = venv {
        print!(
            " ({})",
            Path::new(venv).file_name().unwrap().to_string_lossy()
        );
    }
}

fn write_path_sec(pwd: &Path, repo: Option<&Repo>) {
    print!(" ");
    if repo.is_some() && !is_fs_root(&repo.unwrap().path) {
        print!(
            "{}",
            clr_sym(
                &format!(
                    "@/{}",
                    pwd.strip_prefix(repo.unwrap().path.parent().unwrap())
                        .unwrap()
                        .display()
                ),
                Some(Color::Red)
            )
        );
    } else {
        print!("{}", clr_sym("%~", Some(Color::Red)));
    }
}

fn write_git_sec(repo: Option<&Repo>) {
    if repo.is_none() {
        return;
    }
    let repo = repo.unwrap();
    print!("{}", clr_sym(":«(", Some(Color::Yellow)));
    if let Ref::RebHash(_) = repo.r#ref {
        print!("{}:", clr_sym("@rebase", Some(Color::Magenta)));
    }
    print!(
        "{}{}",
        match &repo.r#ref {
            Ref::Branch(branch) => branch,
            Ref::RebHash(hash) => hash,
        },
        clr_sym(")»", Some(Color::Yellow))
    );
    if repo.is_dirty {
        print!(" {}", clr_sym("✗ ", Some(Color::Cyan)));
    }
}

fn write_dir_acc_sec() {
    if !can_acc_pwd() {
        print!(" {}", clr_sym("", Some(Color::Cyan)));
    }
}

fn write_lprompt() -> Result<(), Error> {
    let cols = term_size()
        .map(|(cols, _)| cols)
        .map_err(|_| Error::new("can not retrieve the terminal dimensions."))?;
    let ip = local_ip().ok();
    let disk_use = disk_use()?;
    let bat_charge = bat_charge()?;
    let time = Local::now();
    let pwd = pwd()?;
    let venv = env_var("VIRTUAL_ENV").ok();
    let repo = git_repo();
    let mut sec_len = SEC_CONST_LEN;
    write_top_sep(cols);
    write_ip_sec(ip.as_ref(), &mut sec_len);
    write_disk_sec(&disk_use, &mut sec_len);
    write_bat_sec(bat_charge.as_ref(), &mut sec_len);
    write_cal_sec(&time);
    write_clk_sec(&time);
    write_mid_sep(cols, sec_len);
    write_root_sec();
    write_exit_code_sec();
    write_venv_sec(venv.as_ref());
    write_path_sec(&pwd, repo.as_ref());
    write_git_sec(repo.as_ref());
    write_dir_acc_sec();
    println!(" ");
    Ok(())
}

fn write_dir_ent(dir_ent: usize, sym: &str, clr: Option<Color>) {
    if dir_ent > 0 {
        print!(
            " {}{}",
            clr_sym(sym, clr),
            dir_ent.to_formatted_string(&Locale::en)
        );
    }
}

fn write_dir_ents_sec(dir_ents: &DirEnts) {
    write_dir_ent(dir_ents.dirs, " ", Some(Color::Yellow));
    write_dir_ent(dir_ents.files, " ", None);
    write_dir_ent(dir_ents.sockets, "󱄙 ", Some(Color::Cyan));
    write_dir_ent(dir_ents.fifos, "󰟦 ", Some(Color::Blue));
    write_dir_ent(dir_ents.blks, "󰇖 ", Some(Color::Magenta));
    write_dir_ent(dir_ents.chars, "󱣴 ", Some(Color::Green));
    write_dir_ent(dir_ents.symlinks, "󰌷 ", Some(Color::Blue));
    write_dir_ent(dir_ents.hiddens, "󰈉 ", Some(Color::Red));
    write_dir_ent(dir_ents.tmps, "󱣹 ", Some(Color::Magenta));
}

fn write_job_sec() {
    print!(
        "{}",
        show_when_job(&format!(" {} {}", clr_sym("", Some(Color::Magenta)), JOBS))
    );
}

fn write_rprompt() -> Result<(), Error> {
    let dir_ents = dir_ents()?;
    write_dir_ents_sec(&dir_ents);
    write_job_sec();
    println!();
    Ok(())
}

fn main() -> ExitCode {
    match parse_args() {
        Ok(res) => match res {
            ArgParse::Opt(Opt::Help) => write_help(),
            ArgParse::Opt(Opt::InitCmdHelp) => write_init_cmd_help(),
            ArgParse::Opt(Opt::PromptCmdHelp) => write_prompt_cmd_help(),
            ArgParse::Opt(Opt::Version) => write_version(),
            ArgParse::Opt(Opt::Repo) => {
                if let Err(err) = open_repo() {
                    write_err(&err);
                    return ExitCode::FAILURE;
                }
            }
            ArgParse::Cmd(Cmd::Init) => init_prompt(),
            ArgParse::Cmd(Cmd::Prompt(side)) => {
                if let Err(err) = match side {
                    Side::Left => write_lprompt(),
                    Side::Right => write_rprompt(),
                } {
                    write_err(&err);
                    return ExitCode::FAILURE;
                }
            }
        },
        Err(err) => {
            write_err(&err);
            return ExitCode::FAILURE;
        }
    }
    ExitCode::SUCCESS
}
