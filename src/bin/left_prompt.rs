use river_dreams::{
	standard_streams::get_output_stream_width,
	styles::{
		Color,
		colorize_text
	},
	symbols::{
		Symbol,
		get_symbol_type_by_environment
	}
};

fn print_commands_separator()
{
	let output_stream_width: u16 = get_output_stream_width();
	for column in 0..output_stream_width
	{
		let is_column_pair: bool = column % 2 == 0;
		if is_column_pair
		{
			print!(
				"{}",
				colorize_text(
					get_symbol_type_by_environment(Symbol {
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
				get_symbol_type_by_environment(Symbol {
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

