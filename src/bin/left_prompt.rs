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
	file_system::disk::MainDisk,
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
	let symbol: PromptString = PromptString::new(
		" ",
		Some("IP "),
		AppearingCondition::Default,
		Color::Blue
	);
	let local_ip_address: String = Network::get_local_ip_address();
	component.push(symbol);
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

fn main()
{
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
	println!(
		"{}",
		prompt
	);
}

