use river_dreams::{
	prompt::
	{
		Prompt,
		PromptComponent
	},
	terminal::Terminal,
	math::is_even
};

fn create_commands_separator_component() -> PromptComponent
{
	let mut body: String = String::new();
	let terminal: Terminal = Terminal::new();
	for column in 0..terminal.get_columns()
	{
		if is_even(column)
		{ body.push_str("a") }
		else
		{ body.push_str("b") }
	}
	PromptComponent::new(body)
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

