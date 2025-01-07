//! A module that contains attributes and functionalities related to the theme prompts.

pub mod left_prompt;
pub mod right_prompt;

use crate::error::Error;

/// Contains the possible prompt sides the theme can write.
#[derive(PartialEq)]
pub enum PromptSide {
    /// Refers to the left prompt.
    Left,
    /// Refers to the right prompt.
    Right,
    /// Refers to no side.
    None,
}

/// Represents a prompt of the theme.
pub trait Prompt {
    /// Writes the prompt.
    ///
    /// # Returns
    /// A result that may contain a possible error encountered.
    fn write() -> Result<(), Error>;
}
