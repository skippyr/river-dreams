use anyhow::{anyhow, bail, Error, Result};
use battery::{units::ratio::ratio, Manager as BatMng, State as RawBatState};
use chrono::{DateTime, Datelike, Local as LocalTime, Timelike};
use crossterm::{style::Stylize, terminal::size as term_size};
use git2::{Repository, RepositoryState, Status as RepoStatus};
use libc::{
    access, closedir, dirent, opendir, readdir, statvfs, strlen, DT_BLK, DT_CHR, DT_DIR, DT_FIFO,
    DT_LNK, DT_REG, DT_SOCK, W_OK,
};
#[cfg(target_os = "macos")]
use libc::{lstat, stat, UF_HIDDEN};
use local_ip_address::local_ip;
use num_format::{Locale, ToFormattedString};
use std::{
    env::{args, current_dir, var as env},
    ffi::c_char,
    fs::read_to_string,
    mem::zeroed,
    path::{Path, PathBuf},
    process::ExitCode,
};

#[cfg(windows)]
const OS: &str = "Windows";
#[cfg(target_os = "macos")]
const OS: &str = "macOS";
#[cfg(target_os = "linux")]
const OS: &str = "Linux";
#[cfg(target_arch = "x86_64")]
const CPU_ARCH: &str = "x86_64";
#[cfg(target_arch = "aarch64")]
const CPU_ARCH: &str = "aarch64";

enum ArgParseResult {
    Ok { is_left: bool },
    Option(ArgOption),
    Err(Error),
}

enum ArgOption {
    Help,
    Version,
}

#[derive(PartialEq)]
enum Color {
    Red = 1,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    None,
}

enum GitRefType {
    Hash(String),
    Branch(String),
}

#[derive(PartialEq)]
enum BatState {
    Discharging,
    Charging,
}

enum FsType {
    Directory,
    Regular,
    Block,
    Character,
    Fifo,
    Socket,
    Link,
}

struct GitRepoInfo {
    path: PathBuf,
    ref_type: GitRefType,
    has_changes: bool,
}

struct BatInfo {
    charge: u8,
    state: BatState,
}

#[derive(Default)]
struct DirStat {
    dir: usize,
    reg: usize,
    hid: usize,
    tmp: usize,
    blk: usize,
    chr: usize,
    fif: usize,
    sck: usize,
    lnk: usize,
}

fn git_repo_info() -> Option<GitRepoInfo> {
    let repo = match Repository::discover(".") {
        Ok(r) => r,
        Err(_) => return None,
    };
    let mut ref_type = None;
    if let Ok(head) = repo.head() {
        if repo.state() == RepositoryState::RebaseInteractive {
            if let Some(hash) = head.target() {
                ref_type = Some(GitRefType::Hash(
                    hash.to_string().chars().take(7).collect::<String>(),
                ));
            }
        } else if let Some(branch) = head.shorthand() {
            ref_type = Some(GitRefType::Branch(String::from(branch)));
        }
    } else {
        if let Ok(head_data) = read_to_string(repo.path().join("HEAD")) {
            if let Some(branch) = head_data.trim().strip_prefix("ref: refs/heads/") {
                ref_type = Some(GitRefType::Branch(branch.to_string()));
            }
        } else if let Ok(cfg) = repo.config() {
            if let Ok(branch) = cfg.get_string("init.defaultBranch") {
                ref_type = Some(GitRefType::Branch(branch));
            }
        }
    }
    let mut has_changes = false;
    if let Ok(statuses) = repo.statuses(None) {
        for ent in &statuses {
            if ent.status().contains(RepoStatus::IGNORED) {
                continue;
            } else if ent.status().intersects(
                RepoStatus::INDEX_NEW
                    | RepoStatus::INDEX_MODIFIED
                    | RepoStatus::INDEX_DELETED
                    | RepoStatus::INDEX_RENAMED
                    | RepoStatus::INDEX_TYPECHANGE
                    | RepoStatus::WT_NEW
                    | RepoStatus::WT_MODIFIED
                    | RepoStatus::WT_DELETED
                    | RepoStatus::WT_TYPECHANGE
                    | RepoStatus::WT_RENAMED,
            ) {
                has_changes = true;
                break;
            }
        }
    }
    Some(GitRepoInfo {
        path: if repo.is_bare() {
            repo.path().to_path_buf()
        } else {
            repo.workdir().unwrap().to_path_buf()
        },
        ref_type: match ref_type {
            Some(r) => r,
            None => GitRefType::Branch(String::from("master")),
        },
        has_changes,
    })
}

fn bat_info() -> Result<Option<BatInfo>> {
    const SUPPLY_ERR: &str = "can not get energy supply info.";
    for b in BatMng::new()
        .map_err(|_| anyhow!(SUPPLY_ERR))?
        .batteries()
        .map_err(|_| anyhow!(SUPPLY_ERR))?
    {
        let b = match b {
            Ok(b) => b,
            Err(_) => continue,
        };
        return Ok(Some(BatInfo {
            charge: (b.state_of_charge().get::<ratio>() * 100.0).round() as u8,
            state: match b.state() {
                RawBatState::Charging | RawBatState::Full | RawBatState::Unknown => {
                    BatState::Charging
                }
                _ => BatState::Discharging,
            },
        }));
    }
    Ok(None)
}

fn is_option(arg: &str) -> bool {
    let c: Vec<char> = arg.chars().collect();
    (c.len() == 2 && c[0] == '-' && c[1].is_ascii_lowercase())
        || (c.len() > 2 && c[0] == '-' && c[1] == '-' && c[2].is_ascii_lowercase())
}

fn is_day_ord(d: &DateTime<LocalTime>, ord: u32) -> bool {
    (d.day() - ord) % 10 == 0
}

fn is_fs_root(p: &Path) -> bool {
    p.ancestors().count() == 1
}

fn is_dft_dir(d: &dirent) -> bool {
    (d.d_name[0] == '.' as c_char && d.d_name[1] == 0)
        || (d.d_name[0] == '.' as c_char && d.d_name[1] == '.' as c_char && d.d_name[2] == 0)
}

fn is_dot_ent(d: &dirent) -> bool {
    d.d_name.starts_with(&['.' as c_char])
}

fn is_tmp_ent(d: &dirent) -> bool {
    d.d_name[unsafe { strlen(d.d_name.as_ptr()) } - 1] == '~' as c_char
}

fn can_access_pwd() -> bool {
    unsafe { access(['.' as c_char, 0].as_ptr(), W_OK) == 0 }
}

fn fs_type_dt(d: &dirent) -> FsType {
    match d.d_type {
        DT_DIR => FsType::Directory,
        DT_REG => FsType::Regular,
        DT_LNK => FsType::Link,
        DT_FIFO => FsType::Fifo,
        DT_SOCK => FsType::Socket,
        DT_CHR => FsType::Character,
        DT_BLK => FsType::Block,
        _ => FsType::Regular,
    }
}

fn num_len(mut n: u16) -> u16 {
    let mut d = if n == 0 { 1 } else { 0 };
    loop {
        if n == 0 {
            break;
        }
        d += 1;
        n /= 10;
    }
    d
}

fn day_ord(d: &DateTime<LocalTime>) -> &'static str {
    if is_day_ord(d, 1) {
        "st"
    } else if is_day_ord(d, 2) {
        "nd"
    } else if is_day_ord(d, 3) {
        "rd"
    } else {
        "th"
    }
}

fn jobs() -> &'static str {
    "%j"
}

fn pwd() -> Result<PathBuf> {
    current_dir().map_err(|_| anyhow!("can not retrieve the current directory."))
}

#[cfg(target_os = "macos")]
fn ent_lstat(d: &dirent) -> Result<*const stat, ()> {
    let mut s = unsafe { zeroed() };
    if unsafe { lstat(d.d_name.as_ptr(), &mut s) } == 0 {
        return Ok(&s);
    }
    Err(())
}

fn pwd_stat() -> Result<DirStat> {
    let dir = unsafe { opendir(['.' as c_char, 0].as_ptr()) };
    if dir.is_null() {
        bail!("can not open the current directory.");
    }
    let mut ds = DirStat::default();
    loop {
        let ent = unsafe { readdir(dir) };
        if ent.is_null() {
            break;
        }
        let ent = unsafe { *ent };
        if is_dft_dir(&ent) {
            continue;
        }
        if is_tmp_ent(&ent) {
            ds.tmp += 1;
        }
        #[cfg(target_os = "macos")]
        if is_dot_ent(&ent)
            || match ent_lstat(&ent) {
                Ok(s) => unsafe { (*s).st_flags & UF_HIDDEN != 0 },
                Err(_) => continue,
            }
        {
            ds.hid += 1;
        }
        #[cfg(target_os = "linux")]
        if is_dot_ent(&ent) {
            ds.hid += 1;
        }
        match fs_type_dt(&ent) {
            FsType::Directory => ds.dir += 1,
            FsType::Regular => ds.reg += 1,
            FsType::Link => ds.lnk += 1,
            FsType::Fifo => ds.fif += 1,
            FsType::Socket => ds.sck += 1,
            FsType::Character => ds.chr += 1,
            FsType::Block => ds.blk += 1,
        }
    }
    unsafe { closedir(dir) };
    Ok(ds)
}

fn disk_use() -> Result<u8> {
    let mut s = unsafe { zeroed() };
    if unsafe { statvfs(['/' as c_char, 0].as_ptr(), &mut s) } != 0 {
        bail!("can not retrieve info about the disk.");
    }
    let t = s.f_frsize * s.f_blocks as u64;
    let a = s.f_frsize * s.f_bavail as u64;
    Ok(((t - a) as f64 / t as f64 * 100.0).round() as u8)
}

fn term_cols() -> Result<u16> {
    term_size()
        .map(|(c, _)| c)
        .map_err(|_| anyhow!("can not retrieve the terminal dimensions."))
}

fn color_sym<T>(sym: T, c: Color) -> String
where
    T: AsRef<str>,
{
    if c == Color::None {
        sym.as_ref().to_string()
    } else {
        format!("%F{{{}}}{}%f", c as u8, sym.as_ref())
    }
}

fn wrap_when_root<T>(m: T) -> String
where
    T: AsRef<str>,
{
    format!("%(#.{}.)", m.as_ref())
}

fn wrap_when_job<T>(m: T) -> String
where
    T: AsRef<str>,
{
    format!("%(1j.{}.)", m.as_ref())
}

fn wrap_in_result<T>(ok: T, err: T) -> String
where
    T: AsRef<str>,
{
    format!("%(?.{}.{})", ok.as_ref(), err.as_ref())
}

fn write_help() {
    println!(
        "{}{}{} {} {} <{}> [{}]...",
        ":".dark_yellow().bold(),
        "x".dark_red().bold(),
        ":".dark_yellow().bold(),
        "Usage:".dark_magenta().bold(),
        "river-dreams".dark_yellow(),
        "SIDE".dark_green().underlined(),
        "OPTIONS".dark_green().underlined()
    );
    println!("Writes a side portion of the shell theme.");
    println!();
    println!("{}", "❡ AVAILABLE SIDES".dark_magenta().bold());
    println!(
        "    {}, {}   refers to the left prompt.",
        "l".dark_green(),
        "left".dark_green()
    );
    println!(
        "    {}, {}  refers to the right prompt.",
        "r".dark_green(),
        "right".dark_green()
    );
    println!();
    println!("{}", "❡ AVAILABLE OPTIONS".dark_magenta().bold());
    println!(
        "    {}, {}     shows the software help instructions.",
        "-h".dark_green(),
        "--help".dark_green()
    );
    println!(
        "    {}, {}  shows the software version.",
        "-v".dark_green(),
        "--version".dark_green()
    );
}

fn write_version() {
    println!(
        "{} v{} {}",
        "river-dreams".dark_yellow(),
        env!("CARGO_PKG_VERSION"),
        format!("({} {})", OS, CPU_ARCH).dark_grey()
    );
    println!(
        "{} <{}>.",
        "Repository:".dark_magenta().bold(),
        "https://github.com/skippyr/river-dreams"
            .dark_blue()
            .underlined()
    );
}

fn write_err(e: &Error) {
    eprintln!(
        "{}{}{} {} {e}",
        ":".dark_yellow().bold(),
        "x".dark_red().bold(),
        ":".dark_yellow().bold(),
        "river-dreams:".dark_magenta().bold(),
    );
    eprintln!(
        "Use {} or {} for help instructions.",
        "-h".dark_green(),
        "--help".dark_green()
    );
}

fn write_top_sep(cols: u16) {
    for i in 0..cols {
        print!(
            "{}",
            if i % 2 == 0 {
                color_sym("≥", Color::Yellow)
            } else {
                color_sym("v", Color::Red)
            }
        );
    }
    print!("{}", color_sym(":«(", Color::Yellow));
}

fn write_ip_mod(ref_cols: &mut u16) {
    let i = local_ip()
        .map(|i| i.to_string())
        .unwrap_or(String::from("No Address Found"));
    print!("{} {}", color_sym(" ", Color::Blue), i);
    *ref_cols += i.len() as u16;
}

fn write_bat_mod(ref_cols: &mut u16) -> Result<()> {
    let b = bat_info()?;
    if b.is_none() {
        return Ok(());
    }
    let b = b.unwrap();
    print!(
        "  {} {}%%",
        match b.charge {
            0..5 => color_sym(
                if b.state == BatState::Charging {
                    "󰢟"
                } else {
                    "󰂎"
                },
                Color::Red,
            ),
            5..30 => color_sym(
                if b.state == BatState::Charging {
                    "󱊤"
                } else {
                    "󱊡"
                },
                Color::Red,
            ),
            30..60 => color_sym(
                if b.state == BatState::Charging {
                    "󱊥"
                } else {
                    "󱊢"
                },
                Color::Yellow,
            ),
            _ => color_sym(
                if b.state == BatState::Charging {
                    "󱊦"
                } else {
                    "󱊣"
                },
                Color::Green,
            ),
        },
        b.charge
    );
    *ref_cols += num_len(b.charge as u16) + 5;
    Ok(())
}

fn write_disk_mod(r#use: u8, ref_cols: &mut u16) {
    print!(
        "  {}{}%%",
        color_sym(
            "󰋊 ",
            match r#use {
                0..70 => Color::Green,
                70..90 => Color::Yellow,
                _ => Color::Red,
            }
        ),
        r#use
    );
    *ref_cols += num_len(r#use as u16);
}

fn write_cal_mod(d: &DateTime<LocalTime>) {
    print!(
        "  {}{}{}",
        color_sym("󰃭 ", Color::Red),
        d.format("(%a) %b %d"),
        day_ord(d)
    );
}

fn write_clock_mod(d: &DateTime<LocalTime>) {
    print!(
        "  {}{}",
        match d.hour() {
            0..6 => color_sym("󰭎 ", Color::Cyan),
            6..12 => color_sym("󰖨 ", Color::Red),
            12..18 => color_sym(" ", Color::Blue),
            _ => color_sym("󰽥 ", Color::Yellow),
        },
        d.format("%Hh%Mm"),
    );
}

fn write_middle_sep(cols: u16, ref_cols: u16) {
    print!("{}", color_sym(")»:", Color::Yellow));
    for i in 0..if cols >= ref_cols { cols - ref_cols } else { 0 } {
        print!(
            "{}",
            if i % 2 == 0 {
                color_sym("-", Color::Red)
            } else {
                color_sym("=", Color::Yellow)
            }
        );
    }
}

fn write_perms_mod() {
    print!(
        "{}",
        wrap_when_root(format!(
            "{}{}{}",
            color_sym("{", Color::Yellow),
            color_sym("#", Color::Red),
            color_sym("}", Color::Yellow)
        ))
    );
}

fn write_exit_code_mod() {
    print!(
        "{}{}{}",
        color_sym("{", Color::Yellow),
        wrap_in_result(color_sym("%?", Color::Yellow), color_sym("%?", Color::Red)),
        color_sym("}⤐ ", Color::Yellow)
    );
}

fn write_venv_mod() {
    if let Ok(v) = env("VIRTUAL_ENV") {
        print!(
            " ({})",
            Path::new(&v).file_name().unwrap().to_string_lossy()
        );
    }
}

fn write_path_mod(pwd: &Path, g: Option<&GitRepoInfo>) {
    print!(" ");
    if g.is_some() && !is_fs_root(&g.unwrap().path) {
        print!(
            "{}",
            color_sym(
                format!(
                    "@/{}",
                    pwd.strip_prefix(g.unwrap().path.parent().unwrap())
                        .unwrap()
                        .display()
                ),
                Color::Red
            )
        );
    } else {
        print!("{}", color_sym("%~", Color::Red));
    }
}

fn write_git_mod(g: Option<&GitRepoInfo>) {
    if g.is_none() {
        return;
    }
    let g = g.unwrap();
    print!("{}", color_sym(":«(", Color::Yellow));
    if let GitRefType::Hash(_) = g.ref_type {
        print!("{}:", color_sym("@rebase", Color::Magenta));
    }
    print!(
        "{}{}",
        match &g.ref_type {
            GitRefType::Branch(b) => b,
            GitRefType::Hash(h) => h,
        },
        color_sym(")»", Color::Yellow)
    );
    if g.has_changes {
        print!(" {}", color_sym("✗ ", Color::Cyan));
    }
}

fn write_acc_mod() {
    if !can_access_pwd() {
        print!(" {}", color_sym("", Color::Cyan));
    }
}

fn write_l_prompt() -> Result<()> {
    const LPROMPT_CONST_LEN: u16 = 42;
    let d = LocalTime::now();
    let c = term_cols()?;
    let u = disk_use()?;
    let g = git_repo_info();
    let p = pwd()?;
    let mut r = LPROMPT_CONST_LEN;
    write_top_sep(c);
    write_ip_mod(&mut r);
    write_bat_mod(&mut r)?;
    write_disk_mod(u, &mut r);
    write_cal_mod(&d);
    write_clock_mod(&d);
    write_middle_sep(c, r);
    write_perms_mod();
    write_exit_code_mod();
    write_venv_mod();
    write_path_mod(&p, g.as_ref());
    write_git_mod(g.as_ref());
    write_acc_mod();
    println!(" ");
    Ok(())
}

fn write_ds_sym(sym: &str, c: Color, s: usize) {
    if s > 0 {
        print!(
            " {}{}",
            color_sym(sym, c),
            s.to_formatted_string(&Locale::en)
        );
    }
}

fn write_ds_mod(d: &DirStat) {
    write_ds_sym(" ", Color::None, d.reg);
    write_ds_sym(" ", Color::Yellow, d.dir);
    write_ds_sym("󰌷 ", Color::Blue, d.lnk);
    write_ds_sym("󰇖 ", Color::Magenta, d.blk);
    write_ds_sym("󱣴 ", Color::Green, d.chr);
    write_ds_sym("󰟦 ", Color::Blue, d.fif);
    write_ds_sym("󱄙 ", Color::Cyan, d.sck);
    write_ds_sym("󰈉 ", Color::Red, d.hid);
    write_ds_sym("󱣹 ", Color::Magenta, d.tmp);
}

fn write_job_mod() {
    print!(
        "{}",
        wrap_when_job(format!(" {}{}", color_sym(" ", Color::Magenta), jobs()))
    );
}

fn write_r_prompt() -> Result<()> {
    let d = pwd_stat()?;
    write_ds_mod(&d);
    write_job_mod();
    println!();
    Ok(())
}

fn parse_args() -> ArgParseResult {
    let args: Vec<String> = args().skip(1).collect();
    for arg in &args {
        if arg == "-h" || arg == "--help" {
            return ArgParseResult::Option(ArgOption::Help);
        } else if arg == "-v" || arg == "--version" {
            return ArgParseResult::Option(ArgOption::Version);
        } else if is_option(arg) {
            return ArgParseResult::Err(anyhow!(r#"invalid option "{arg}" provided."#));
        }
    }
    if args.len() == 0 {
        return ArgParseResult::Err(anyhow!("no side provided."));
    }
    let mut is_left = false;
    if args[0] == "l" || args[0] == "left" {
        is_left = true;
    } else if args[0] != "r" && args[0] != "right" {
        return ArgParseResult::Err(anyhow!(r#"invalid side "{}" provided."#, args[0]));
    }
    ArgParseResult::Ok { is_left }
}

fn main() -> ExitCode {
    match parse_args() {
        ArgParseResult::Ok { is_left } => {
            if let Err(e) = if is_left {
                write_l_prompt()
            } else {
                write_r_prompt()
            } {
                write_err(&e);
                return ExitCode::FAILURE;
            }
        }
        ArgParseResult::Option(o) => match o {
            ArgOption::Help => write_help(),
            ArgOption::Version => write_version(),
        },
        ArgParseResult::Err(e) => {
            write_err(&e);
            return ExitCode::FAILURE;
        }
    }
    return ExitCode::SUCCESS;
}
