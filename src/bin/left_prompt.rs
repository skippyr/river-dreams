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
	math::MathOperations, network::Network
};

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
		Color::Yellow
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
	let mut component: PromptComponent = PromptComponent::new();
	component.push(Network::get_local_ip_address());
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.push(create_commands_separator_component());
	left_prompt.push(create_top_left_connector_component());
	left_prompt.push(create_local_ip_address_component());
	println!(
		"{}",
		left_prompt
	);
}

