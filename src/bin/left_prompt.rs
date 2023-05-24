use river_dreams::
{
	styles::Color,
	prompt::
	{
		AppearingCondition,
		Prompt,
		PromptComponent,
		PromptString
	},
	terminal::TerminalEmulator,
	math::Math,
	network::Network,
	file_system::
	{
		disk::MainDisk,
		git::Repository,
		paths::
		{
			Paths,
			PathTreater,
			PathsPermissions
		}
	},
	time::
	{
		Calendar,
		Clock,
		DayMoment
	}
};

pub fn create_vertical_separator_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	for column in 0..TerminalEmulator::get_columns()
	{
		let symbol: PromptString =
			if Math::is_even(column)
			{
				PromptString::new(
					"⟐",
					Some("*"),
					AppearingCondition::Default,
					Color::Red
				)
			}
			else
			{
				PromptString::new(
					"",
					Some("X"),
					AppearingCondition::Default,
					Color::Default
				)
			};
		component.push(symbol);
	}
	component
}

fn create_horizontal_separator_component() -> PromptComponent
{
	PromptComponent::from(PromptString::new(
		"  ",
		None::<String>,
		AppearingCondition::Default,
		Color::Red
	))
}

fn create_top_left_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let connector: PromptString = PromptString::new(
		":«",
		None::<String>,
		AppearingCondition::Default,
		Color::Yellow
	);
	let compass_rose: PromptString = PromptString::new(
		"󱎂  ",
		Some("X "),
		AppearingCondition::Default,
		Color::Red
	);
	component.push(connector);
	component.push(compass_rose);
	component
}

fn create_top_right_connector_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let connector: PromptString = PromptString::new(
		"»:\n",
		None::<String>,
		AppearingCondition::Default,
		Color::Yellow
	);
	let compass_rose: PromptString = PromptString::new(
		" 󱎂 ",
		Some(" X"),
		AppearingCondition::Default,
		Color::Red
	);
	component.push(compass_rose);
	component.push(connector);
	component
}

fn create_local_ip_address_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let color: Color = Color::Blue;
	let symbol: PromptString = PromptString::new(
		" ",
		Some("IP "),
		AppearingCondition::Default,
		color
	);
	let host: String = String::from("%m");
	let separator: PromptString = PromptString::new(
		"@",
		None::<String>,
		AppearingCondition::Default,
		color
	);
	let local_ip_address: String = Network::get_local_ip_address();
	component.push(symbol);
	component.push(host);
	component.push(separator);
	component.push(local_ip_address);
	component
}

fn create_disk_usage_percentage_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		"󰋊 ",
		Some("Disk "),
		AppearingCondition::Default,
		Color::Yellow
	);
	let usage_percentage: String = format!(
		"{}%%",
		MainDisk::get_usage_percentage()
	);
	component.push(symbol);
	component.push(usage_percentage);
	component
}

fn create_calendar_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let symbol: PromptString = PromptString::new(
		"󰸗 ",
		Some("Calendar "),
		AppearingCondition::Default,
		Color::Red
	);
	component.push(symbol);
	component.push(Calendar::from_current_moment());
	component
}

fn create_clock_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let clock: Clock = Clock::from_current_moment();
	let symbol: PromptString =
		match clock.get_day_moment()
		{
			DayMoment::Dawn =>
			{
				PromptString::new(
					" ",
					Some("Dawn "),
					AppearingCondition::Default,
					Color::Cyan
				)
			}
			DayMoment::Morning =>
			{
				PromptString::new(
					"󰖨 ",
					Some("Morning "),
					AppearingCondition::Default,
					Color::Red
				)
			}
			DayMoment::Afternoon =>
			{
				PromptString::new(
					" ",
					Some("Afternoon "),
					AppearingCondition::Default,
					Color::Blue
				)
			}
			DayMoment::Night =>
			{
				PromptString::new(
					"󰽥 ",
					Some("Night "),
					AppearingCondition::Default,
					Color::Yellow
				)
			}
		};
	component.push(symbol);
	component.push(clock);
	component
}

fn create_root_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let curly_brackets_color: Color = Color::Yellow;
	let left_curly_bracket: PromptString = PromptString::new(
		"{",
		None::<String>,
		AppearingCondition::Default,
		curly_brackets_color
	);
	let right_curly_bracket: PromptString = PromptString::new(
		"}",
		None::<String>,
		AppearingCondition::Default,
		curly_brackets_color
	);
	let symbol: PromptString = PromptString::new(
		"#",
		None::<String>,
		AppearingCondition::Default,
		Color::Red
	);
	let root: PromptString = PromptString::new(
		format!(
			"{}{}{}",
			left_curly_bracket,
			symbol,
			right_curly_bracket
		),
		None::<String>,
		AppearingCondition::OnRootUser,
		Color::Default
	);
	component.push(root);
	component
}

fn create_arrow_component() -> PromptComponent
{
	PromptComponent::from(PromptString::new(
		"⤐  ",
		Some("~> "),
		AppearingCondition::Default,
		Color::Yellow
	))
}

fn create_virtual_environment_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	if let Some(virtual_environment) = Paths::get_virtual_environment()
	{
		let curly_brackets_color: Color = Color::Cyan;
		let left_curly_bracket: PromptString = PromptString::new(
			"(",
			None::<String>,
			AppearingCondition::Default,
			curly_brackets_color
		);
		let right_curly_bracket: PromptString = PromptString::new(
			") ",
			None::<String>,
			AppearingCondition::Default,
			curly_brackets_color
		);
		component.push(left_curly_bracket);
		component.push(PathTreater::get_base_name(&virtual_environment));
		component.push(right_curly_bracket);
	}
	component
}

fn create_directory_component(repository: &Option<Repository>) -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	let directory: PromptString = PromptString::new(
	PathTreater::abbreviate(
			&Paths::get_current_directory(),
			repository
		),
		None::<String>,
		AppearingCondition::Default,
		Color::Red
	);
	component.push(directory);
	component
}

fn create_git_component(repository: &Option<Repository>) -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	if let Some(repository) = repository
	{
		let left_connector: PromptString = PromptString::new(
			":«",
			None::<String>,
			AppearingCondition::Default,
			Color::Green
		);
		let right_connector: PromptString = PromptString::new(
			"»",
			None::<String>,
			AppearingCondition::Default,
			Color::Green
		);
		let branch: String = repository.get_branch().get_name();
		component.push(left_connector);
		component.push(branch);
		component.push(right_connector);
	}
	component
}

fn create_directory_ownership_component() -> PromptComponent
{
	let mut component: PromptComponent = PromptComponent::new();
	if !PathsPermissions::does_user_owns_current_directory()
	{
		let symbol: PromptString = PromptString::new(
			" ",
			Some(" #"),
			AppearingCondition::Default,
			Color::Magenta
		);
		component.push(symbol);
	}
	component
}

fn create_cursor_component() -> PromptComponent
{
	PromptComponent::from(PromptString::new(
		" ✗",
		Some(" X"),
		AppearingCondition::Default,
		Color::Cyan
	))
}

fn main()
{
	let repository: Option<Repository> = Repository::from_current_directory();
	let mut prompt: Prompt = Prompt::new();
	prompt.push(create_vertical_separator_component());
	prompt.push(create_top_left_connector_component());
	prompt.push(create_local_ip_address_component());
	prompt.push(create_horizontal_separator_component());
	prompt.push(create_disk_usage_percentage_component());
	prompt.push(create_horizontal_separator_component());
	prompt.push(create_calendar_component());
	prompt.push(create_horizontal_separator_component());
	prompt.push(create_clock_component());
	prompt.push(create_top_right_connector_component());
	prompt.push(create_root_component());
	prompt.push(create_arrow_component());
	prompt.push(create_virtual_environment_component());
	prompt.push(create_directory_component(&repository));
	prompt.push(create_git_component(&repository));
	prompt.push(create_directory_ownership_component());
	prompt.push(create_cursor_component());
	prompt.push(create_horizontal_separator_component());
	print!(
		"{}",
		prompt
	);
}

