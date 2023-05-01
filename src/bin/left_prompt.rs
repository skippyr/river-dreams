use river_dreams::
{
	standard_streams::get_output_stream_width,
	styles::
	{
		Color,
		colorize
	},
	symbols::
	{
		Symbol,
		get_commands_separator_symbol,
		get_clock_symbol
	},
	time::
	{
		get_calendar_string,
		get_clock_string
	}
};
use chrono::
{
	DateTime,
	Local
};

fn print_top_left_decorator()
{
	let decorator: Symbol = Symbol
	{
		default_text: String::from("╭─"),
		fallback_text: String::from("┌─"),
		color: Color::Red
	};
	print!(
		"{}{}",
		colorize(
			decorator.get_text_for_environment(),
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
	let decorator: Symbol = Symbol
	{
		default_text: String::from("╰"),
		fallback_text: String::from("└"),
		color: Color::Red
	};
	print!(
		"{}",
		colorize(
			decorator.get_text_for_environment(),
			decorator.color
		)
	);
}

fn print_commands_separator()
{
	for column in 0..get_output_stream_width()
	{
		let symbol: Symbol = get_commands_separator_symbol(column);
		print!(
			"{}",
			colorize(
				symbol.get_text_for_environment(),
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
	let symbol: Symbol = Symbol
	{
		default_text: String::from(" "),
		fallback_text: String::from("Calendar "),
		color: Color::Red
	};
	print!(
		"{}{}",
		colorize(
			symbol.get_text_for_environment(),
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
			symbol.get_text_for_environment(),
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

