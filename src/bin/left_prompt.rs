use river_dreams::{
	prompt::
	{
		Prompt,
		PromptComponent
	},
	label::Label,
	styles::Color,
	terminal::Terminal,
	math::is_even,
};

fn create_commands_separator_component() -> PromptComponent
{
	let mut structure: String = String::new();
	let terminal: Terminal = Terminal::new();
	for column in 0..terminal.get_columns()
	{
		let mut label: Label = Label::new();
		if is_even(column)
		{
			label.set_fallback_text(String::from("="));
			label.set_default_text(String::from(""));
			label.set_color(Color::Red);
		}
		else
		{
			label.set_fallback_text(String::from("-"));
			label.set_default_text(String::from(""));
			label.set_color(Color::Default);
		}
		structure.push_str(&label.as_string());
	}
	PromptComponent::from(structure)
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.add_component(create_commands_separator_component());
	print!(
		"{}",
		left_prompt.as_string()
	);
}

