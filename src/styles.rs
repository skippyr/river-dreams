//! A module to work with styles for the ZSH shell specifically.

/// A set of possible ANSI colors a string can be.
#[derive(Clone, Copy)]
pub enum Color {
    /// The default colors of the terminal emulator's theme.
    Default,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
}

impl Color {
    /// Returns a formatter that represents the color for the ZSH shell.
    pub fn get_style_sequence(&self) -> String {
        match self {
            Self::Default => String::from("%f"),
            Self::Black => String::from("%F{black}"),
            Self::Red => String::from("%F{red}"),
            Self::Green => String::from("%F{green}"),
            Self::Yellow => String::from("%F{yellow}"),
            Self::Blue => String::from("%F{blue}"),
            Self::Magenta => String::from("%F{magenta}"),
            Self::Cyan => String::from("%F{cyan}"),
            Self::White => String::from("%F{white}"),
        }
    }
}
