use river_dreams::{
	standard_streams::get_output_stream_width,
	styles::{
		Color,
		colorize_string
	},
	symbols::{
		Symbol,
		get_symbol_string
	},
	math::is_pair
};

fn print_commands_separator()
{
	for column in 0..get_output_stream_width()
	{
		print!(
			"{}",
			if is_pair(column)
			{
				colorize_string(
					get_symbol_string(Symbol {
						default: String::from(""),
						fallback: String::from("=")
					}),
					Color::Red
				)
			}
			else
			{
				get_symbol_string(Symbol {
					default: String::from(""),
					fallback: String::from("-")
				})
			}
		)
	}
}

fn main()
{
	print_commands_separator();
}

