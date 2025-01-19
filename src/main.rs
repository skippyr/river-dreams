use crossterm::style::Stylize;
use open::that as open_in_browser;
use std::{
    env::args,
    fmt::{Display, Error as FmtError, Formatter},
    process::ExitCode,
};

const NAME: &str = env!("CARGO_PKG_NAME");
const VERSION: &str = env!("CARGO_PKG_VERSION");
const REPO: &str = env!("CARGO_PKG_HOMEPAGE");

enum ValOpt<T> {
    Some(T),
    None,
    Invalid,
}

enum Opt {
    Help,
    InitCmdHelp,
    PromptCmdHelp,
    Version,
    Repo,
}

enum Cmd {
    Init,
    Prompt(PromptSide),
}

enum ArgParseResult {
    Opt(Opt),
    Cmd(Cmd),
}

#[derive(Default)]
enum PromptSide {
    #[default]
    Left,
    Right,
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

fn parse_args() -> Result<ArgParseResult, Error> {
    let args = args().skip(1).collect::<Vec<_>>();
    let mut cmd = ValOpt::None;
    if let Some(arg) = args.first() {
        if arg == "init" {
            cmd = ValOpt::Some(Cmd::Init);
        } else if arg == "prompt" {
            cmd = ValOpt::Some(Cmd::Prompt(PromptSide::default()));
        } else {
            cmd = ValOpt::Invalid;
        }
    }
    for arg in &args {
        if arg == "-h" || arg == "--help" {
            match cmd {
                ValOpt::Some(Cmd::Init) => return Ok(ArgParseResult::Opt(Opt::InitCmdHelp)),
                ValOpt::Some(Cmd::Prompt(_)) => return Ok(ArgParseResult::Opt(Opt::PromptCmdHelp)),
                _ => return Ok(ArgParseResult::Opt(Opt::Help)),
            }
        } else if arg == "-v" || arg == "--version" {
            return Ok(ArgParseResult::Opt(Opt::Version));
        } else if arg == "-g" || arg == "--repo" {
            return Ok(ArgParseResult::Opt(Opt::Repo));
        } else if is_opt(arg) {
            return Err(Error::new(format_args!(
                r#"invalid option "{arg}" provided."#
            )));
        } else if is_ill_opt(arg) {
            return Err(Error::new(format_args!(
                r#"malformed option "{arg}" provided."#
            )));
        }
    }
    match cmd {
        ValOpt::Some(Cmd::Init) => return Ok(ArgParseResult::Cmd(Cmd::Init)),
        ValOpt::None => return Err(Error::new("no command provided.")),
        ValOpt::Invalid => {
            return Err(Error::new(format_args!(
                r#"invalid command "{}" provided."#,
                args[0]
            )))
        }
        _ => {}
    }
    let mut side = PromptSide::Left;
    if let Some(arg) = args.get(1) {
        if arg == "right" {
            side = PromptSide::Right;
        } else if arg != "left" {
            return Err(Error::new(r#"invalid prompt side "{arg}" provided."#));
        }
    } else {
        return Err(Error::new("no prompt side provided."));
    }
    Ok(ArgParseResult::Cmd(Cmd::Prompt(side)))
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
    eprintln!(
        "{} use {} or {} for help instructions.",
        "  info:".dark_cyan().bold(),
        "-h".dark_cyan(),
        "--help".dark_cyan()
    );
}

fn write_help() {
    todo!("write_help");
}

fn write_init_cmd_help() {
    todo!("write_init_cmd_help");
}

fn write_prompt_cmd_help() {
    todo!("write_prompt_cmd_help");
}

fn write_version() {
    todo!("write_version");
}

fn init_prompt() {
    todo!("init_prompt");
}

fn write_lprompt() -> Result<(), Error> {
    todo!("write_lprompt");
    Ok(())
}

fn write_rprompt() -> Result<(), Error> {
    todo!("write_rprompt");
    Ok(())
}

fn main() -> ExitCode {
    match parse_args() {
        Ok(res) => match res {
            ArgParseResult::Opt(Opt::Help) => write_help(),
            ArgParseResult::Opt(Opt::InitCmdHelp) => write_init_cmd_help(),
            ArgParseResult::Opt(Opt::PromptCmdHelp) => write_prompt_cmd_help(),
            ArgParseResult::Opt(Opt::Version) => write_version(),
            ArgParseResult::Opt(Opt::Repo) => {
                if let Err(err) = open_repo() {
                    write_err(&err);
                    return ExitCode::FAILURE;
                }
            }
            ArgParseResult::Cmd(Cmd::Init) => init_prompt(),
            ArgParseResult::Cmd(Cmd::Prompt(side)) => {
                if let Err(err) = match side {
                    PromptSide::Left => write_lprompt(),
                    PromptSide::Right => write_rprompt(),
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
