//! A module that contains attributes and functionalities to manipulate and parse arguments.

use crate::{error::Error, prompt::PromptSide};

/// Contains the flags the theme can parse.
pub enum Flag {
    /// Indicates the software help instruction should be written.
    Help,
    /// Indicates the software version should be written.
    Version,
    /// Indicates the shell connector script should be written.
    Connector,
}

impl Flag {
    /// Checks whether an argument is a flag.
    ///
    /// # Parameters
    /// - `argument`: the argument to be checked.
    ///
    /// # Returns
    /// A boolean that states the argument is a flag.
    fn is(argument: &str) -> bool {
        let graphemes: Vec<char> = argument.chars().collect();
        (graphemes.len() == 2 && graphemes[0] == '-' && graphemes[1].is_ascii_lowercase())
            || (graphemes.len() > 2
                && graphemes[0] == '-'
                && graphemes[1] == '-'
                && graphemes[2].is_ascii_lowercase())
    }
}

/// Contains the possible types of arguments the theme can parse.
pub enum ArgumentType {
    /// Attributes that may change aspects of its main execution.
    Attributes {
        /// The side of the prompt to be written.
        prompt_side: PromptSide,
    },
    /// A flag that makes the theme perform an exclusive and alternate action.
    Flag(
        /// The flag parsed.
        Flag,
    ),
}

/// Represents a handler to manipulate arguments.
pub struct Arguments;

impl Arguments {
    /// Parses the arguments provided.
    ///
    /// # Parameters
    /// - `arguments`: the arguments to be iterated and parsed.
    ///
    /// # Returns
    /// The argument type parsed or an error.
    ///
    /// # Errors
    /// An error happens if:
    /// - An argument is an unrecognized flag.
    /// - An invalid prompt side is provided.
    /// - No prompt side is provided.
    pub fn parse<T>(arguments: T) -> Result<ArgumentType, Error>
    where
        T: IntoIterator<Item = String>,
    {
        let mut prompt_side: PromptSide = PromptSide::None;
        for argument in arguments.into_iter().skip(1) {
            if argument == "-h" || argument == "--help" {
                return Ok(ArgumentType::Flag(Flag::Help));
            } else if argument == "-v" || argument == "--version" {
                return Ok(ArgumentType::Flag(Flag::Version));
            } else if argument == "--connector" {
                return Ok(ArgumentType::Flag(Flag::Connector));
            } else if Flag::is(&argument) {
                return Err(Error::new(format!(
                    r#"invalid flag "{argument}" provided."#
                )));
            }
            if argument == "l" || argument == "left" {
                prompt_side = PromptSide::Left;
            } else if argument == "r" || argument == "right" {
                prompt_side = PromptSide::Right;
            } else {
                return Err(Error::new(format!(
                    r#"invalid side "{argument}" provided."#
                )));
            }
        }
        if prompt_side == PromptSide::None {
            return Err(Error::new("no prompt side provided."));
        }
        Ok(ArgumentType::Attributes { prompt_side })
    }
}
