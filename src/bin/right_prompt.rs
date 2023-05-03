use river_dreams::
{
	prompt::
	{
		Prompt,
		PromptComponent
	},
	styles::Color,
	text::TextWithFallback,
	file_system::DirectoryEntryTypes
};

fn create_directory_entry_types_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut component_structure_as_vec: Vec<String> = Vec::new();
	let directory_entry_types: DirectoryEntryTypes = DirectoryEntryTypes::from_pwd();
	if directory_entry_types.get_quantity_of_executables() > 0
	{
		let mut symbol: TextWithFallback = TextWithFallback::new();
		symbol.set_fallback_content(String::from("Executables "));
		symbol.set_default_content(String::from(" "));
		symbol.set_color(Color::Green);
		component_structure_as_vec.push(format!(
			"{}{}",
			symbol.as_string(),
			directory_entry_types.get_quantity_of_executables()
		));
	}
	if directory_entry_types.get_quantity_of_hidden_files() > 0
	{
		let mut symbol: TextWithFallback = TextWithFallback::new();
		symbol.set_fallback_content(String::from("Hiddens "));
		symbol.set_default_content(String::from(" "));
		symbol.set_color(Color::Red);
		component_structure_as_vec.push(format!(
			"{}{}",
			symbol.as_string(),
			directory_entry_types.get_quantity_of_hidden_files()
		));
	}
	if directory_entry_types.get_quantity_of_symlinks() > 0
	{
		let mut symbol: TextWithFallback = TextWithFallback::new();
		symbol.set_fallback_content(String::from("Symlinks "));
		symbol.set_default_content(String::from(" "));
		symbol.set_color(Color::Blue);
		component_structure_as_vec.push(format!(
			"{}{}",
			symbol.as_string(),
			directory_entry_types.get_quantity_of_symlinks()
		));
	}
	component.append_string_to_structure(component_structure_as_vec.join(" "));
	component
}

fn main()
{
	let mut right_prompt: Prompt = Prompt::new();
	right_prompt.add_component(create_directory_entry_types_component());
	print!(
		"{}",
		right_prompt.as_string()
	);
}

