use river_dreams::{
	standard_streams::get_output_stream_width,
	styles::{
		Color,
		colorize_string
	},
	symbols::{
		Symbol,
		get_symbol_string,
		get_colorized_clock_symbol
	},
	math::is_pair,
	time::{
		get_calendar_string,
		get_clock_string
	}
};
use chrono::{
	DateTime,
	Local
};

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

fn print_top_right_decorator()
{
	println!(
		"{}",
		colorize_string(
			String::from("}"),
			Color::Yellow
		)
	);
}

fn print_bottom_left_decorator()
{
	print!(
		"{}",
		colorize_string(
			get_symbol_string(Symbol {
				default: String::from("╰"),
				fallback: String::from("└")
			}),
			Color::Red
		)
	);
}

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

fn print_separator()
{
	print!(
		" {} ",
		colorize_string(
			String::from("¦"),
			Color::Red
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

fn print_clock(local_time: &DateTime<Local>)
{
	print!(
		"{}{}",
		get_colorized_clock_symbol(local_time),
		get_clock_string(local_time)
	);
}

fn main()
{
	let local_time: DateTime<Local> = Local::now();
	print_commands_separator();
	print_top_left_decorator();
	print_calendar(&local_time);
	print_separator();
	print_clock(&local_time);
	print_top_right_decorator();
	print_bottom_left_decorator();
}

