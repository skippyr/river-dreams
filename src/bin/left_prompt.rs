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
		PathAbbreviation,
		get_virtual_env_as_path_buff
	},
	git::GitRepository
};
use local_ip_address::local_ip;

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
			symbol.set_default_content(String::from("⟐"));
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

fn create_local_ip_address_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let color: Color = Color::Blue;
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_fallback_content(String::from("IP "));
	symbol.set_default_content(String::from(" "));
	symbol.set_color(color.clone());
	let host_name: String = String::from("%m");
	let mut separator: Text = Text::new();
	separator.set_content(String::from("@"));
	separator.set_color(color.clone());
	let local_ip_address: String = match local_ip()
	{
		Ok(local_ip_address) =>
		{ local_ip_address.to_string() }
		Err(_error) =>
		{ String::from("No Address Found") }
	};
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(host_name);
	component.append_string_to_structure(separator.as_string());
	component.append_string_to_structure(local_ip_address);
	component
}

fn create_calendar_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let calendar: Calendar = Calendar::from_current_moment();
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_fallback_content(String::from("Calendar "));
	symbol.set_default_content(String::from(" "));
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
			symbol.set_default_content(String::from("󰖨 "));
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

fn create_bottom_left_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut connector: TextWithFallback = TextWithFallback::new();
	connector.set_fallback_content(String::from("└"));
	connector.set_default_content(String::from("╰"));
	connector.set_color(Color::Red);
	component.append_string_to_structure(connector.as_string());
	component
}

fn create_shell_status_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut left_curly_bracket: Text = Text::new();
	left_curly_bracket.set_content(String::from("{"));
	left_curly_bracket.set_color(Color::Cyan);
	let mut right_curly_bracket: Text = Text::new();
	right_curly_bracket.set_content(String::from("}"));
	right_curly_bracket.set_color(Color::Cyan);
	let mut error_symbol: TextWithFallback = TextWithFallback::new();
	error_symbol.set_fallback_content(String::from("X "));
	error_symbol.set_default_content(String::from(" "));
	error_symbol.set_color(Color::Red);
	let mut exit_code: Text = Text::new();
	exit_code.set_content(format!(
		"{}{}%?{}",
		left_curly_bracket.as_string(),
		error_symbol.as_string(),
		right_curly_bracket.as_string()
	));
	let mut arrow: TextWithFallback = TextWithFallback::new();
	arrow.set_fallback_content(String::from("> "));
	arrow.set_default_content(String::from("⤐ "));
	arrow.set_color(Color::Cyan);
	component.append_string_to_structure(exit_code.as_error_string());
	component.append_string_to_structure(arrow.as_string());
	component
}

fn create_user_component() -> PromptComponent
{
	let color: Color = Color::Blue;
	let mut component: PromptComponent = PromptComponent::new();
	let prefix: String = String::from("as ");
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_default_content(String::from(" "));
	symbol.set_color(color.clone());
	let mut user: Text = Text::new();
	user.set_content(String::from("%n"));
	user.set_color(color.clone());
	component.append_string_to_structure(prefix);
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(user.as_string());
	component
}

fn create_virtual_env_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	if let Some(virtual_env) = get_virtual_env_as_path_buff()
	{
		let color: Color = Color::Magenta;
		let prefix: String = String::from(" using ");
		let mut symbol: TextWithFallback = TextWithFallback::new();
		symbol.set_default_content(String::from(" "));
		symbol.set_color(color.clone());
		let mut virtual_env_as_text: Text = Text::new();
		virtual_env_as_text.set_content(virtual_env.as_abbreviated_string());
		virtual_env_as_text.set_color(color.clone());
		component.append_string_to_structure(prefix);
		component.append_string_to_structure(symbol.as_string());
		component.append_string_to_structure(virtual_env_as_text.as_string());
	}
	component
}

fn create_pwd_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let color: Color = Color::Red;
	let prefix: String = String::from(" at ");
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_default_content(String::from(" "));
	symbol.set_color(color.clone());
	let mut pwd_abbreviated: Text = Text::new();
	pwd_abbreviated.set_content(get_pwd_as_path_buff().as_abbreviated_string());
	pwd_abbreviated.set_color(color.clone());
	component.append_string_to_structure(prefix);
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(pwd_abbreviated.as_string());
	component
}

fn create_git_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	if let Some(git_repository) = GitRepository::from_pwd()
	{
		let color: Color = Color::Green;
		let prefix: String = String::from(" on ");
		let mut symbol: TextWithFallback = TextWithFallback::new();
		symbol.set_default_content(String::from("󰘬 "));
		symbol.set_color(color.clone());
		let mut branch: Text = Text::new();
		branch.set_content(git_repository.get_branch().get_name());
		branch.set_color(color.clone());
		component.append_string_to_structure(prefix);
		component.append_string_to_structure(symbol.as_string());
		component.append_string_to_structure(branch.as_string());
	};
	component
}

fn create_cursor_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut symbol: TextWithFallback = TextWithFallback::new();
	symbol.set_fallback_content(String::from(" X  "));
	symbol.set_default_content(String::from(" 🗶  "));
	symbol.set_color(Color::Cyan);
	component.append_string_to_structure(symbol.as_string());
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.add_component(create_commands_separator_component());
	left_prompt.add_component(create_top_left_connector_component());
	left_prompt.add_component(create_local_ip_address_component());
	left_prompt.add_component(create_horizontal_separator_component());
	left_prompt.add_component(create_disk_component());
	left_prompt.add_component(create_horizontal_separator_component());
	left_prompt.add_component(create_calendar_component());
	left_prompt.add_component(create_horizontal_separator_component());
	left_prompt.add_component(create_clock_component());
	left_prompt.add_component(create_top_right_connector_component());
	left_prompt.add_component(create_bottom_left_connector_component());
	left_prompt.add_component(create_shell_status_component());
	left_prompt.add_component(create_user_component());
	left_prompt.add_component(create_virtual_env_component());
	left_prompt.add_component(create_pwd_component());
	left_prompt.add_component(create_git_component());
	left_prompt.add_component(create_cursor_component());
	print!(
		"{}",
		left_prompt.as_string()
	);
}

