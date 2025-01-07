//! A module that contains functionalities to handle and emit errors related to the runtime of the theme.

use crate::metadata::ThemeMetadata;
use crossterm::style::Stylize;

/// Represents a generic runtime error throwed by the theme.
#[derive(Debug)]
pub struct Error {
    /// A message that describes the error.
    message: String,
}

impl Error {
    /// Creates an instance of the `Error` struct with a given message.
    ///
    /// # Parameters
    /// - `message`: the message that describes the error.
    ///
    /// # Returns
    /// An instance of the `Error` struct.
    pub fn new<T>(message: T) -> Self
    where
        T: Into<String>,
    {
        Self {
            message: message.into(),
        }
    }

    /// Writes the error to the terminal error stream.
    pub fn write(&self) {
        eprintln!(
            "{} {}",
            format!("{}:", ThemeMetadata::name()).dark_magenta().bold(),
            self.message
        );
        eprintln!(
            "Use {} or {} for help instructions.",
            "-h".dark_green(),
            "--help".dark_green()
        );
    }
}

impl std::fmt::Display for Error {
    /// Formats the error message for display.
    ///
    /// # Parameters
    /// - `formatter`: the formatter to be used.
    ///
    /// # Returns
    /// A result with a possible error encountered.
    fn fmt(&self, formatter: &mut std::fmt::Formatter) -> Result<(), std::fmt::Error> {
        write!(formatter, "{}", self.message)
    }
}

impl std::error::Error for Error {}
