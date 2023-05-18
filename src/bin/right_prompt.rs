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
};

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
	prompt.push(create_job_component());
	print!(
		"{}",
		prompt
	);
}

