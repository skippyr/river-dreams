use river_dreams::
{
	prompt::
	{
		Prompt,
		PromptComponent,
		PromptString,
		JobString
	},
	styles::Color,
	file_system::directory::DirectoryEntryTypes,
	locale::NumberFormatter
};

fn create_directory_entry_types_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut structure: Vec<String> = Vec::new();
	let entry_types: DirectoryEntryTypes = DirectoryEntryTypes::from_pwd();
	if entry_types.get_quantity_of_executable_files() > 0
	{
		let symbol: PromptString = PromptString::new(
			String::from(" "),
			Some(String::from("Executable ")),
			Color::Green
		);
		structure.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_executable_files())
		));
	}
	if entry_types.get_quantity_of_hidden_files() > 0
	{
		let symbol: PromptString = PromptString::new(
			String::from(" "),
			Some(String::from("Hidden ")),
			Color::Red
		);
		structure.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_hidden_files())
		));
	}
	if entry_types.get_quantity_of_symlinks() > 0
	{
		let symbol: PromptString = PromptString::new(
			String::from(" "),
			Some(String::from("Symlink ")),
			Color::Blue
		);
		structure.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_symlinks())
		));
	}
	component.push(structure.join(" "));
	component
}

fn create_jobs_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from("  "),
		Some(String::from(" Job ")),
		Color::Magenta
	);
	let jobs: JobString = JobString::new(format!(
		"{}%j",
		symbol
	));
	component.push(format!(
		"{}",
		jobs
	));
	component
}

fn main()
{
	let mut right_prompt: Prompt = Prompt::new();
	right_prompt.push(create_directory_entry_types_component());
	right_prompt.push(create_jobs_component());
	print!(
		"{}",
		right_prompt
	);
}

