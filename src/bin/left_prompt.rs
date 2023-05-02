use river_dreams::{
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
};

fn create_commands_separator_component() -> PromptComponent
{
	let mut structure: String = String::new();
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
		structure.push_str(&symbol.as_string());
	}
	PromptComponent::from(structure)
}

fn create_top_left_connector_component() -> PromptComponent
{
	let mut structure: String = String::new();
	let mut connector: TextWithFallback = TextWithFallback::new();
	connector.set_fallback_content(String::from("┌"));
	connector.set_default_content(String::from("╭"));
	connector.set_color(Color::Red);
	structure.push_str(&connector.as_string());
	let mut curly_brackets: Text = Text::new();
	curly_brackets.set_content(String::from("{"));
	curly_brackets.set_color(Color::Yellow);
	structure.push_str(&curly_brackets.as_string());
	PromptComponent::from(structure)
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.add_component(create_commands_separator_component());
	left_prompt.add_component(create_top_left_connector_component());
	print!(
		"{}",
		left_prompt.as_string()
	);
}

