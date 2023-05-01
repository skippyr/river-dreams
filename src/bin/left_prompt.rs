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
	let output_stream_width: u16 = get_output_stream_width();
	for column in 0..output_stream_width
	{
		if is_pair(column)
		{
			print!(
				"{}",
				colorize_string(
					get_symbol_string(Symbol {
						default: String::from(""),
						fallback: String::from("=")
					}),
					Color::Red
				)
			);
		}
		else
		{
			print!(
				"{}",
				get_symbol_string(Symbol {
					default: String::from(""),
					fallback: String::from("-")
				})
			);
		}
	}
}

fn main()
{
	print_commands_separator();
}

