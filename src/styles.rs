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
	Normal
}

fn get_color_sequence(color: Color) -> String
{
	format!(
		"%F{{{}}}",
		match color
		{
			Color::Black =>
			{ "black" }
			Color::Red =>
			{ "red" }
			Color::Green =>
			{ "green" }
			Color::Yellow =>
			{ "yellow" }
			Color::Blue =>
			{ "blue" }
			Color::Magenta =>
			{ "magenta" }
			Color::Cyan =>
			{ "cyan" }
			Color::White =>
			{ "white" }
			Color::Normal =>
			{ "normal" }
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

