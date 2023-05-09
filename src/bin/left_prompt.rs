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
	math::MathOperations
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

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.push(create_commands_separator_component());
	println!(
		"{}",
		left_prompt
	);
}

