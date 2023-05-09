use river_dreams::{
	prompt::
	{
		Prompt,
		PromptComponent,
		PromptString, JobString
	},
	styles::Color
};

fn create_jobs_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from("  "),
		Some(String::from(" Jobs ")),
		Color::Green
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
	right_prompt.push(create_jobs_component());
	print!(
		"{}",
		right_prompt
	);
}

