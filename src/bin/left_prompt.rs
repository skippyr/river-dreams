use river_dreams::
{
	styles::Color,
	prompt::
	{
		AppearingCondition,
		Prompt,
		PromptComponent,
		PromptString
	},
	terminal::TerminalEmulator,
	math::Math
};

pub fn create_vertical_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	for column in 0..TerminalEmulator::get_columns()
	{
		let symbol: PromptString =
			if Math::is_even(column)
			{
				PromptString::new(
					"⟐",
					Some("*"),
					AppearingCondition::None,
					Color::Red
				)
			}
			else
			{
				PromptString::new(
					"",
					Some("X"),
					AppearingCondition::None,
					Color::Default
				)
			};
		component.push(symbol);
	}
	component
}

fn main()
{
	let mut prompt: Prompt = Prompt::new();
	prompt.push(create_vertical_separator_component());
	println!(
		"{}",
		prompt
	)
}

