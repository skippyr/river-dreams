//! A module that contains attributes and funcionalities related specifically to the ZSH shell.

/// Contains the colors that can be applied in the ZSH shell.
pub enum Color {
    /// The red color.
    Red,
    /// The green color.
    Green,
    /// The yellow color.
    Yellow,
    /// The blue color.
    Blue,
    /// The magenta color.
    Magenta,
    /// The cyan color.
    Cyan,
}

impl Color {
    /// Gets the ANSI value of the shell color.
    ///
    /// # Returns
    /// The ANSI value of the shell color.
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

/// Represents the ZSH shell.
pub struct Zsh;

impl Zsh {
    /// The percentage symbol escaped using the special syntax of the shell.
    pub const PERCENTAGE_SYMBOL: &str = "%%";
    /// The path to the current directory using the special syntax of the shell.
    pub const CURRENT_DIRECTORY: &str = "%~";
    /// The exit code of the last command inserted in the shell using its special syntax.
    pub const EXIT_CODE: &str = "%?";
    /// The total number of backgrounds jobs running in the shell using its special syntax.
    pub const TOTAL_JOBS: &str = "%j";

    /// Adds color to a symbol by wrapping it in the special syntax of the shell.
    ///
    /// # Parameters
    /// - `symbol`: the symbol to be used.
    /// - `color`: the color to be applied or nothing.
    ///
    /// # Returns
    /// The symbol using the color specificied.
    pub fn color_symbol<T>(symbol: T, color: Option<Color>) -> String
    where
        T: std::fmt::Display,
    {
        match color {
            Some(color) => format!("%F{{{}}}{symbol}%f", color.ansi()),
            None => symbol.to_string(),
        }
    }

    /// Wraps a symbol using the special syntax of the shell to only make it visible when the user is `root`.
    ///
    /// # Parameters
    /// - `symbol`: the symbol to be used.
    ///
    /// # Returns
    /// The symbol only visible when the user is `root`.
    pub fn show_symbol_when_root<T>(symbol: T) -> String
    where
        T: std::fmt::Display,
    {
        format!("%(#.{symbol}.)")
    }

    /// Wraps two symbols using the special syntax of the shell to only make them visible when the last command inserted in the shell succeds and fails.
    ///
    /// # Parameters
    /// - `success_symbol`: the symbol to be used when the command succeds.
    /// - `fail_symbol`: the symbol to be used when the command fails.
    pub fn show_symbols_in_exit_codes<T>(success_symbol: T, fail_symbol: T) -> String
    where
        T: std::fmt::Display,
    {
        format!("%(?.{success_symbol}.{fail_symbol})")
    }

    /// Wraps a symbol using the special syntax of the shell to only make it visible when there are background jobs.
    ///
    /// # Parameters
    /// - `symbol`: the symbol to be used.
    pub fn show_symbol_when_job<T>(symbol: T) -> String
    where
        T: std::fmt::Display,
    {
        format!("%(1j.{symbol}.)")
    }
}
