use river_dreams::{
	standard_streams::get_output_stream_width,
	styles::{
		Color,
		colorize
	},
	symbols::{
		Symbol,
		choose_symbol_string_for_environment,
		get_clock_symbol
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
	let decorator: Symbol = Symbol {
		default: String::from("╭─"),
		fallback: String::from("┌─"),
		color: Color::Red
	};
	print!(
		"{}{}",
		colorize(
			choose_symbol_string_for_environment(&decorator),
			decorator.color
		),
		colorize(
			String::from("{"),
			Color::Yellow
		)
	);
}

fn print_top_right_decorator()
{
	println!(
		"{}",
		colorize(
			String::from("}"),
			Color::Yellow
		)
	);
}

fn print_bottom_left_decorator()
{
	let decorator: Symbol = Symbol {
		default: String::from("╰"),
		fallback: String::from("└"),
		color: Color::Red
	};
	print!(
		"{}",
		colorize(
			choose_symbol_string_for_environment(&decorator),
			decorator.color
		)
	);
}

fn print_commands_separator()
{
	for column in 0..get_output_stream_width()
	{
		let symbol: Symbol = if is_pair(column)
		{
			Symbol
			{
				default: String::from(""),
				fallback: String::from("="),
				color: Color::Red
			}
		}
		else
		{
			Symbol {
				default: String::from(""),
				fallback: String::from("-"),
				color: Color::Normal
			}
		};
		print!(
			"{}",
			colorize(
				choose_symbol_string_for_environment(&symbol),
				symbol.color
			)
		);
	}
}

fn print_separator()
{
	print!(
		" {} ",
		colorize(
			String::from("¦"),
			Color::Red
		)
	);
}

fn print_calendar(local_time: &DateTime<Local>)
{
	let symbol: Symbol = Symbol {
		default: String::from(" "),
		fallback: String::from("Calendar "),
		color: Color::Red
	};
	print!(
		"{}{}",
		colorize(
			choose_symbol_string_for_environment(&symbol),
			symbol.color
		),
		get_calendar_string(local_time)
	);
}

fn print_clock(local_time: &DateTime<Local>)
{
	let symbol: Symbol = get_clock_symbol(local_time);
	print!(
		"{}{}",
		colorize(
			choose_symbol_string_for_environment(&symbol),
			symbol.color
		),
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

