use river_dreams::
{
	terminal::get_terminal_width,
	shell::
	{
		get_exit_code_string,
		get_user
	},
	styles::
	{
		Color,
		colorize,
		get_status_color
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
	},
	file_system::
	{
		get_disk_usage_percentage,
		get_current_directory_path_abbreviated
	}, environment_variables::get_python_environment
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
	for column in 0..get_terminal_width()
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

fn print_horizontal_separator()
{
	print!(
		" {} ",
		colorize(
			String::from("¦"),
			Color::Red
		)
	);
}

fn print_disk_usage_percentage()
{
	let symbol: Symbol = Symbol
	{
		default_text: String::from(" "),
		fallback_text: String::from("Disk "),
		color: Color::Yellow
	};
	print!(
		"{}{}%%",
		colorize(
			symbol.get_text_for_environment(),
			symbol.color
		),
		get_disk_usage_percentage()
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

fn print_shell_status()
{
	let error_symbol: Symbol = Symbol
	{
		default_text: String::from(" "),
		fallback_text: String::from("X "),
		color: Color::Red
	};
	let arrow_symbol: Symbol = Symbol
	{
		default_text: String::from("⤐  "),
		fallback_text: String::from("> "),
		color: get_status_color()
	};
	print!(
		"{}{}",
		get_exit_code_string(&error_symbol),
		colorize(
			arrow_symbol.get_text_for_environment(),
			arrow_symbol.color
		)
	);
}

fn print_user()
{
	print!(
		"{} ",
		colorize(
			get_user(),
			Color::Cyan
		)
	);
}

fn print_python_environment()
{
	let python_environment: String = match get_python_environment()
	{
		Some(python_environment) =>
		{ python_environment }
		None =>
		{ return; }
	};
	print!(
		"using {} ",
		colorize(
			python_environment,
			Color::Magenta
		)
	);
}

fn print_current_directory_path_abbreviated()
{
	print!(
		"at {}",
		colorize(
			get_current_directory_path_abbreviated(),
			Color::Green
		)
	);
}

fn main()
{
	let local_time: DateTime<Local> = Local::now();
	print_commands_separator();
	print_top_left_decorator();
	print_disk_usage_percentage();
	print_horizontal_separator();
	print_calendar(&local_time);
	print_horizontal_separator();
	print_clock(&local_time);
	print_top_right_decorator();
	print_bottom_left_decorator();
	print_shell_status();
	print_user();
	print_python_environment();
	print_current_directory_path_abbreviated();
}

