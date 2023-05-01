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
	math::is_pair,
	time::get_calendar_string
};
use chrono::{
	DateTime,
	Local
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
		);
	}
}

fn print_top_left_decorator()
{
	print!(
		"{}{}",
		colorize_string(
			get_symbol_string(Symbol {
				default: String::from("╭─"),
				fallback: String::from("┌─")
			}),
			Color::Red
		),
		colorize_string(
			String::from("{"),
			Color::Yellow
		)
	);
}

fn print_calendar(local_time: &DateTime<Local>)
{
	print!(
		"{}{}",
		colorize_string(
			get_symbol_string(Symbol {
				default: String::from(" "),
				fallback: String::from("Calendar ")
			}),
			Color::Red
		),
		get_calendar_string(local_time)
	);
}

fn main()
{
	let local_time: DateTime<Local> = Local::now();
	print_commands_separator();
	print_top_left_decorator();
	print_calendar(&local_time);
}

