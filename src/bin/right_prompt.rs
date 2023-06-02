use river_dreams::{
	styles::Color,
	prompt::{
		Prompt,
		PromptComponent,
		PromptString,
		AppearingCondition
	},
	file_system::paths::PathEntryTypes,
	locale::NumberFormatter
};

fn create_entry_types_component() -> PromptComponent {
	let mut formatter: Vec<String> = Vec::new();
	let entry_types: PathEntryTypes = PathEntryTypes::from_current_directory();
	if entry_types.get_quantity_of_executable_files() > 0 {
		let symbol: PromptString = PromptString::new(
			"󱖏 ",
			Some("EXE "),
			AppearingCondition::Default,
			Color::Green
		);
		formatter.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_executable_files())
		));
	}
	if entry_types.get_quantity_of_symlinks() > 0 {
		let symbol: PromptString = PromptString::new(
			"󰌷 ",
			Some("SYL "),
			AppearingCondition::Default,
			Color::Blue
		);
		formatter.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_symlinks())
		));
	}
	if entry_types.get_quantity_of_hidden_files() > 0 {
		let symbol: PromptString = PromptString::new(
			" ",
			Some("HID "),
			AppearingCondition::Default,
			Color::Red
		);
		formatter.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_hidden_files())
		));
	}
	if entry_types.get_quantity_of_broken_files() > 0 {
		let symbol: PromptString = PromptString::new(
			"󰻝 ",
			Some("BRK "),
			AppearingCondition::Default,
			Color::Yellow
		);
		formatter.push(format!(
			"{}{}",
			symbol,
			NumberFormatter::format_u32(entry_types.get_quantity_of_broken_files())
		));
	}
	PromptComponent::from(formatter.join("  "))
}

fn create_job_component() -> PromptComponent {
	let symbol: PromptString = PromptString::new(
		"   ",
		Some("  Job "),
		AppearingCondition::Default,
		Color::Magenta
	);
	PromptComponent::from(PromptString::new(
		format!(
			"{}{}",
			symbol,
			"%j"
		),
		None::<String>,
		AppearingCondition::OnJob,
		Color::Default
	))
}

fn main() {
	let mut prompt: Prompt = Prompt::new();
	prompt.push(create_entry_types_component());
	prompt.push(create_job_component());
	print!(
		"{}",
		prompt
	);
}

