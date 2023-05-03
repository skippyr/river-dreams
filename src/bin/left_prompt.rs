use river_dreams::
{
	prompt::
	{
		Prompt,
		PromptComponent
	},
	text::
	{
		Text,
		TextWithFallback
	},
	styles::Color,
	terminal::Terminal,
	math::is_even,
	time::
	{
		Calendar,
		Clock,
		DayMoment
	},
	file_system::
	{
		MainDisk,
		get_pwd_as_path_buff,
		PathAbbreviation
	}
};

fn create_horizontal_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut symbol: Text = Text::new();
	symbol.set_content(String::from(" ¦ "));
	symbol.set_color(Color::Red);
	component.append_string_to_structure(symbol.as_string());
	component
}

fn create_commands_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let terminal: Terminal = Terminal::new();
	for column in 0..terminal.get_columns()
	{
		let mut symbol: TextWithFallback = TextWithFallback::new();
		if is_even(column)
		{
			symbol.set_fallback_content(String::from("="));
			symbol.set_default_content(String::from(""));
			symbol.set_color(Color::Red);
		}
		else
		{
			symbol.set_fallback_content(String::from("-"));
			symbol.set_default_content(String::from(""));
			symbol.set_color(Color::Default);
		}
		component.append_string_to_structure(symbol.as_string());
	}
	component
}

fn create_top_left_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut connector: TextWithFallback = TextWithFallback::new();
	connector.set_fallback_content(String::from("┌"));
	connector.set_default_content(String::from("╭"));
	connector.set_color(Color::Red);
	component.append_string_to_structure(connector.as_string());
	let mut left_curly_bracket: Text = Text::new();
	left_curly_bracket.set_content(String::from("{"));
	left_curly_bracket.set_color(Color::Yellow);
	component.append_string_to_structure(left_curly_bracket.as_string());
	component
}

fn create_calendar_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let calendar: Calendar = Calendar::from_current_moment();
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_fallback_content(String::from("Calendar "));
	symbol.set_default_content(String::from(" "));
	symbol.set_color(Color::Red);
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(calendar.as_string());
	component
}

fn create_clock_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let clock: Clock = Clock::from_current_moment();
	let mut symbol: TextWithFallback = TextWithFallback::new();
	match clock.get_day_moment()
	{
		DayMoment::Dawn =>
		{
			symbol.set_fallback_content(String::from("Dawn "));
			symbol.set_default_content(String::from(" "));
			symbol.set_color(Color::Cyan);
		}
		DayMoment::Morning =>
		{
			symbol.set_fallback_content(String::from("Morning "));
			symbol.set_default_content(String::from(" "));
			symbol.set_color(Color::Red);
		}
		DayMoment::Afternoon =>
		{
			symbol.set_fallback_content(String::from("Afternoon "));
			symbol.set_default_content(String::from(" "));
			symbol.set_color(Color::Blue);
		}
		DayMoment::Night =>
		{
			symbol.set_fallback_content(String::from("Night "));
			symbol.set_default_content(String::from("󰽥 "));
			symbol.set_color(Color::Yellow);
		}
	}
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(clock.as_string());
	component
}

fn create_disk_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_fallback_content(String::from("Disk "));
	symbol.set_default_content(String::from("󰋊 "));
	symbol.set_color(Color::Yellow);
	let main_disk: MainDisk = MainDisk::new();
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(main_disk.get_in_use_percentage_as_string());
	component
}

fn create_top_right_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut right_curly_bracket: Text = Text::new();
	right_curly_bracket.set_content(String::from("}\n"));
	right_curly_bracket.set_color(Color::Yellow);
	component.append_string_to_structure(right_curly_bracket.as_string());
	component
}

fn create_pwd_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut pwd: Text = Text::new();
	pwd.set_content(get_pwd_as_path_buff().as_abbreviated_string());
	pwd.set_color(Color::Green);
	component.append_string_to_structure(pwd.as_string());
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.add_component(create_commands_separator_component());
	left_prompt.add_component(create_top_left_connector_component());
	left_prompt.add_component(create_disk_component());
	left_prompt.add_component(create_horizontal_separator_component());
	left_prompt.add_component(create_calendar_component());
	left_prompt.add_component(create_horizontal_separator_component());
	left_prompt.add_component(create_clock_component());
	left_prompt.add_component(create_top_right_connector_component());
	left_prompt.add_component(create_pwd_component());
	print!(
		"{}",
		left_prompt.as_string()
	);
}

