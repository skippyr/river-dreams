use river_dreams::
{
	styles::Color,
	prompt::
	{
		Prompt,
		PromptComponent,
		PromptString
	},
	terminal::TerminalEmulator,
	math::MathOperations,
	network::Network,
	time::
	{
		Calendar,
		Clock,
		DayMoment
	}
};

fn create_horizontal_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let separator: PromptString = PromptString::new(
		String::from(" ¦ "),
		None,
		Color::Red
	);
	component.push(format!(
		"{}",
		separator
	));
	component
}

fn create_commands_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let terminal_emulator: TerminalEmulator = TerminalEmulator::from_environment();
	for column in 0..terminal_emulator.get_columns()
	{
		if MathOperations::is_even(column)
		{
			component.push(format!(
				"{}",
				PromptString::new(
					String::from("⟐"),
					Some(String::from("=")),
					Color::Red
				)
			))
		}
		else
		{
			component.push(format!(
				"{}",
				PromptString::new(
					String::from(""),
					Some(String::from("-")),
					Color::Default
				)
			))
		}
	}
	component
}

fn create_top_left_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let connector: PromptString = PromptString::new(
		String::from("╭"),
		Some(String::from("┌")),
		Color::Red
	);
	let left_curly_brackets: PromptString = PromptString::new(
		String::from("{"),
		None,
		Color::Cyan
	);
	component.push(
		format!(
			"{}{}",
			connector,
			left_curly_brackets
		)
	);
	component
}

fn create_local_ip_address_component() -> PromptComponent
{
	let color: Color = Color::Blue;
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from(" "),
		Some(String::from("IP ")),
		color.clone()
	);
	let host: String = String::from("%m");
	let separator: PromptString = PromptString::new(
		String::from("@"),
		None,
		color.clone()
	);
	component.push(
		format!(
			"{}{}{}{}",
			symbol,
			host,
			separator,
			Network::get_local_ip_address()
		)
	);
	component
}

fn create_calendar_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from("󰨲 "),
		Some(String::from("Calendar ")),
		Color::Red
	);
	let calendar: Calendar = Calendar::from_current_moment();
	component.push(format!(
		"{}{}",
		symbol,
		calendar
	));
	component
}

fn create_clock_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let clock: Clock = Clock::from_current_moment();
	let symbol: PromptString = match clock.get_day_moment()
	{
		DayMoment::Dawn =>
		{
			PromptString::new(
				String::from(" "),
				Some(String::from("Dawn ")),
				Color::Cyan
			)
		}
		DayMoment::Morning =>
		{
			PromptString::new(
				String::from("󰖨 "),
				Some(String::from("Morning ")),
				Color::Red
			)
		}
		DayMoment::Afternoon =>
		{
			PromptString::new(
				String::from(" "),
				Some(String::from("Afternoon ")),
				Color::Blue
			)
		}
		DayMoment::Night =>
		{
			PromptString::new(
				String::from("󰽥 "),
				Some(String::from("Night ")),
				Color::Yellow
			)
		}
	};
	component.push(format!(
		"{}{}",
		symbol,
		clock
	));
	component
}

pub fn create_top_right_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let right_curly_brackets: PromptString = PromptString::new(
		String::from("}\n"),
		None,
		Color::Cyan
	);
	component.push(format!(
		"{}",
		right_curly_brackets
	));
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.push(create_commands_separator_component());
	left_prompt.push(create_top_left_connector_component());
	left_prompt.push(create_local_ip_address_component());
	left_prompt.push(create_horizontal_separator_component());
	left_prompt.push(create_calendar_component());
	left_prompt.push(create_horizontal_separator_component());
	left_prompt.push(create_clock_component());
	left_prompt.push(create_top_right_connector_component());
	println!(
		"{}",
		left_prompt
	);
}

