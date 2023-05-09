pub enum Color
{
	Default,
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Other(String)
}

impl Color
{
	pub fn get_escape_sequence(&self) -> String
	{
		match self
		{
			Color::Default =>
			{ String::from("%f") }
			Color::Black =>
			{ String::from("%F{black}") }
			Color::Red =>
			{ String::from("%F{red}") }
			Color::Green =>
			{ String::from("%F{green}") }
			Color::Yellow =>
			{ String::from("%F{yellow}") }
			Color::Blue =>
			{ String::from("%F{blue}") }
			Color::Magenta =>
			{ String::from("%F{magenta}") }
			Color::Cyan =>
			{ String::from("%F{cyan}") }
			Color::White =>
			{ String::from("%F{white}") }
			Color::Other(escape_sequence) =>
			{ escape_sequence.clone() }
		}
	}
}


