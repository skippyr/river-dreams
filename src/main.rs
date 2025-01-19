use std::{
    env::args,
    fmt::{Display, Error as FmtError, Formatter},
};

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
    let args = args().collect::<Vec<_>>();
    let mut cmd = ValOpt::None;
    if let Some(arg) = args.first() {
        if arg == "init" {
            cmd = ValOpt::Some(Cmd::Init);
        } else {
            cmd = ValOpt::Some(Cmd::Prompt(PromptSide::default()));
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
                r#"unrecognized option "{arg}" provided."#
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
    Err(Error::new("test"))
}

fn main() {}
