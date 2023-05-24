use river_dreams::
{
	styles::Color,
	prompt::
	{
		Prompt,
		PromptComponent,
		PromptString,
		AppearingCondition
	},
	file_system::paths::PathEntryTypes,
	locale::NumberFormatter
};

fn create_entry_types_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let mut structure: Vec<String> = Vec::new();
	let entry_types: PathEntryTypes = PathEntryTypes::from_current_directory();
	if entry_types.get_quantity_of_executable_files() > 0
	{
		let symbol: PromptString = PromptString::new(
			"󱖏 ",
			Some("Executable "),
			AppearingCondition::Default,
			Color::Green
		);
		let executable_files: String = format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_executable_files())
		);
		structure.push(executable_files);
	}
	if entry_types.get_quantity_of_symlinks() > 0
	{
		let symbol: PromptString = PromptString::new(
			"󰌷 ",
			Some("Symlink "),
			AppearingCondition::Default,
			Color::Blue
		);
		let symlinks: String = format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_symlinks())
		);
		structure.push(symlinks);
	}
	if entry_types.get_quantity_of_hidden_files() > 0
	{
		let symbol: PromptString = PromptString::new(
			" ",
			Some("Hidden "),
			AppearingCondition::Default,
			Color::Red
		);
		let broken_files: String = format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_hidden_files())
		);
		structure.push(broken_files);
	}
	if entry_types.get_quantity_of_broken_files() > 0
	{
		let symbol: PromptString = PromptString::new(
			"󰻝 ",
			Some("Broken "),
			AppearingCondition::Default,
			Color::Yellow
		);
		let broken_files: String = format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_broken_files())
		);
		structure.push(broken_files);
	}
	component.push(structure.join("  "));
	component
}

fn create_job_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		"   ",
		Some("  Job "),
		AppearingCondition::Default,
		Color::Magenta
	);
	let job: PromptString = PromptString::new(
		format!(
			"{}{}",
			symbol,
			"%j"
		),
		None::<String>,
		AppearingCondition::OnJob,
		Color::Default
	);
	component.push(job);
	component
}

fn main()
{
	let mut prompt: Prompt = Prompt::new();
	prompt.push(create_entry_types_component());
	prompt.push(create_job_component());
	print!(
		"{}",
		prompt
	);
}

