use river_dreams::
{
	styles::Color,
	prompt::
	{
		Prompt,
		PromptComponent,
		PromptString,
		RootString
	},
	terminal::TerminalEmulator,
	math::MathOperations,
	network::Network,
	time::
	{
		Calendar,
		Clock,
		DayMoment
	},
	file_system::
	{
		disks::MainDisk,
		paths::Paths,
	},
	git::GitRepository
};

fn create_horizontal_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let separator: PromptString = PromptString::new(
		String::from(" :: "),
		None,
		Color::Red
	);
	component.push(format!(
		"{}",
		separator
	));
	component
}

fn create_commands_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let terminal_emulator: TerminalEmulator = TerminalEmulator::from_environment();
	for column in 0..terminal_emulator.get_columns()
	{
		if MathOperations::is_even(column)
		{
			component.push(format!(
				"{}",
				PromptString::new(
					String::from("⟐"),
					Some(String::from("*")),
					Color::Red
				)
			))
		}
		else
		{
			component.push(format!(
				"{}",
				PromptString::new(
					String::from(""),
					Some(String::from("X")),
					Color::Default
				)
			))
		}
	}
	component
}

fn create_top_left_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let connector: PromptString = PromptString::new(
		String::from(":"),
		None,
		Color::Red
	);
	let left_curly_bracket: PromptString = PromptString::new(
		String::from("{"),
		None,
		Color::Yellow
	);
	component.push(
		format!(
			"{}{}",
			connector,
			left_curly_bracket
		)
	);
	component
}

fn create_local_ip_address_component() -> PromptComponent
{
	let color: Color = Color::Blue;
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from(" "),
		Some(String::from("IP ")),
		color.clone()
	);
	component.push(
		format!(
			"{}{}",
			symbol,
			Network::get_local_ip_address()
		)
	);
	component
}

fn create_disk_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from("󰋊 "),
		Some(String::from("Disk ")),
		Color::Yellow
	);
	let disk: MainDisk = MainDisk::from_environment();
	component.push(format!(
		"{}{}%%",
		symbol,
		disk.get_in_use_percentage()
	));
	component
}

fn create_calendar_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		String::from("󰨲 "),
		Some(String::from("Calendar ")),
		Color::Red
	);
	let calendar: Calendar = Calendar::from_current_moment();
	component.push(format!(
		"{}{}",
		symbol,
		calendar
	));
	component
}

fn create_clock_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let clock: Clock = Clock::from_current_moment();
	let symbol: PromptString = match clock.get_day_moment()
	{
		DayMoment::Dawn =>
		{
			PromptString::new(
				String::from(" "),
				Some(String::from("Dawn ")),
				Color::Cyan
			)
		}
		DayMoment::Morning =>
		{
			PromptString::new(
				String::from("󰖨 "),
				Some(String::from("Morning ")),
				Color::Red
			)
		}
		DayMoment::Afternoon =>
		{
			PromptString::new(
				String::from(" "),
				Some(String::from("Afternoon ")),
				Color::Blue
			)
		}
		DayMoment::Night =>
		{
			PromptString::new(
				String::from("󰽥 "),
				Some(String::from("Night ")),
				Color::Yellow
			)
		}
	};
	component.push(format!(
		"{}{}",
		symbol,
		clock
	));
	component
}

pub fn create_top_right_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let right_curly_bracket: PromptString = PromptString::new(
		String::from("}\n"),
		None,
		Color::Yellow
	);
	component.push(format!(
		"{}",
		right_curly_bracket
	));
	component
}

pub fn create_root_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let left_curly_bracket: PromptString = PromptString::new(
		String::from("{"),
		None,
		Color::Yellow
	);
	let right_curly_bracket: PromptString = PromptString::new(
		String::from("}"),
		None,
		Color::Yellow
	);
	let symbol: PromptString = PromptString::new(
		String::from("#"),
		None,
		Color::Red
	);
	let root: RootString = RootString::new(format!(
		"{}{}{}",
		left_curly_bracket,
		symbol,
		right_curly_bracket
	));
	component.push(format!(
		"{}",
		root
	));
	component
}

pub fn create_arrow_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let arrow: PromptString = PromptString::new(
		String::from("⤐  "),
		Some(String::from("> ")),
		Color::Yellow
	);
	component.push(format!(
		"{}",
		arrow
	));
	component
}

pub fn create_virtual_env_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let virtual_env: Option<String> = Paths::get_virtual_env();
	if let Some(virtual_env) = virtual_env
	{
		let prefix: PromptString = PromptString::new(
			String::from("("),
			None,
			Color::Magenta
		);
		let suffix: PromptString = PromptString::new(
			String::from(")"),
			None,
			Color::Magenta
		);
		component.push(format!(
			"{}{}{} ",
			prefix,
			virtual_env,
			suffix
		));
	};
	component
}

pub fn create_directory_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let directory: PromptString = PromptString::new(
		String::from("%1~"),
		None,
		Color::Red
	);
	component.push(format!(
		"{}",
		directory
	));
	component
}

pub fn create_git_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let repository: Option<GitRepository> = GitRepository::from_pwd();
	if let Some(repository) = repository
	{
		let prefix: PromptString = PromptString::new(
			String::from("::«"),
			None,
			Color::Green
		);
		let suffix: PromptString = PromptString::new(
			String::from("»"),
			None,
			Color::Green
		);
		let branch: String = repository.get_branch().get_name();
		component.push(format!(
			"{}{}{}",
			prefix,
			branch,
			suffix
		));
	};
	component
}

fn main()
{
	let mut left_prompt: Prompt = Prompt::new();
	left_prompt.push(create_commands_separator_component());
	left_prompt.push(create_top_left_connector_component());
	left_prompt.push(create_local_ip_address_component());
	left_prompt.push(create_horizontal_separator_component());
	left_prompt.push(create_disk_component());
	left_prompt.push(create_horizontal_separator_component());
	left_prompt.push(create_calendar_component());
	left_prompt.push(create_horizontal_separator_component());
	left_prompt.push(create_clock_component());
	left_prompt.push(create_top_right_connector_component());
	left_prompt.push(create_root_component());
	left_prompt.push(create_arrow_component());
	left_prompt.push(create_virtual_env_component());
	left_prompt.push(create_directory_component());
	left_prompt.push(create_git_component());
	print!(
		"{}",
		left_prompt
	);
}

