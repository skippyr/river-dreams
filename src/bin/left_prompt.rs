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
	math::is_even, time::Calendar,
};

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
	symbol.set_default_content(String::from(" "));
	symbol.set_fallback_content(String::from("Calendar "));
	symbol.set_color(Color::Red);
	component.append_string_to_structure(symbol.as_string());
	component.append_string_to_structure(calendar.as_string());
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.add_component(create_commands_separator_component());
	left_prompt.add_component(create_top_left_connector_component());
	left_prompt.add_component(create_calendar_component());
	print!(
		"{}",
		left_prompt.as_string()
	);
}

