use std::fmt::{Display, Formatter, Error as FmtError};

enum Opt {
    Help,
    InitCmdHelp,
    WriteCmdHelp,
    Version,
    Repo,
}

enum Cmd {
    Init,
    Write(PromptSide),
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

fn parse_args() -> Result<ArgParseResult, Error> {
    Err(Error::new("test"))
}

fn main() {
    
}
