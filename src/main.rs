use river_dreams::{
    arguments::{ArgumentType, Arguments, Flag},
    metadata::{write_connector, write_help, write_version},
    prompt::{left_prompt::LeftPrompt, right_prompt::RightPrompt, Prompt, PromptSide},
};
use std::process::ExitCode;

fn main() -> ExitCode {
    match Arguments::parse(std::env::args()) {
        Ok(argument_type) => match argument_type {
            ArgumentType::Attributes { prompt_side } => {
                if let Err(error) = if prompt_side == PromptSide::Left {
                    LeftPrompt::write()
                } else {
                    RightPrompt::write()
                } {
                    error.write();
                    return ExitCode::FAILURE;
                }
            }
            ArgumentType::Flag(flag) => match flag {
                Flag::Help => write_help(),
                Flag::Version => write_version(),
                Flag::Connector => write_connector(),
            },
        },
        Err(error) => {
            error.write();
            return ExitCode::FAILURE;
        }
    }
    return ExitCode::SUCCESS;
}
