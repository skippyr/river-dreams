#[derive(Clone, Copy)]
pub enum Color {
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
