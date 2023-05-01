pub enum Color
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Normal,
	Other(String)
}

fn get_color_sequence(color: Color) -> String
{
	format!(
		"%F{{{}}}",
		match color
		{
			Color::Black =>
			{ String::from("black") }
			Color::Red =>
			{ String::from("red") }
			Color::Green =>
			{ String::from("green") }
			Color::Yellow =>
			{ String::from("yellow") }
			Color::Blue =>
			{ String::from("blue") }
			Color::Magenta =>
			{ String::from("magenta") }
			Color::Cyan =>
			{ String::from("cyan") }
			Color::White =>
			{ String::from("white") }
			Color::Normal =>
			{ String::from("normal") }
			Color::Other(color) =>
			{ color }
		}
	)
}

pub fn colorize(
	text: String,
	color: Color
) -> String
{
	format!(
		"{}{}%f",
		get_color_sequence(color),
		text
	)
}

