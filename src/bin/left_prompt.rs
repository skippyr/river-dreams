//! The binary file that will be used as the left prompt. It will set the
//! ${PROMPT} prompt variable value.

use river_dreams::{
    date_time::{Calendar, Clock, DayMoment},
    file_system::{
        disk::MainDisk,
        git::Repository,
        paths::{PathTreater, Paths, PathsPermissions},
    },
    math::Math,
    network::Network,
    prompt::{AppearingCondition, Prompt, PromptComponent, PromptString},
    styles::Color,
    terminal::TerminalEmulator,
};

/// Returns the prompt component that will be used to separate the prompt from
/// the output of the previous command.
pub fn create_vertical_separator_component() -> PromptComponent {
    let mut component: PromptComponent = PromptComponent::new();
    for column in 0..TerminalEmulator::get_columns() {
        let symbol: PromptString = if Math::is_even(column) {
            PromptString::new("⊵", Some("%%"), AppearingCondition::Default, Color::Yellow)
        } else {
            PromptString::new("⊼", Some(":"), AppearingCondition::Default, Color::Red)
        };
        component.push(symbol);
    }
    component
}

/// Returns a prompt component that will be used to separate other components
/// of the prompt.
fn create_horizontal_separator_component() -> PromptComponent {
    PromptComponent::from(PromptString::new(
        "  ",
        None::<String>,
        AppearingCondition::Default,
        Color::Default,
    ))
}

/// Returns the prompt component that will be used as the top left connector
/// decorator of the prompt.
fn create_top_left_connector_component() -> PromptComponent {
    PromptComponent::from(PromptString::new(
        ":«",
        None::<String>,
        AppearingCondition::Default,
        Color::Yellow,
    ))
}

/// Returns the prompt component that will be used as the top right connector
/// decorator of the prompt. It contains a new line to break the remaining of the
/// prompt to the next line.
fn create_top_right_connector_component() -> PromptComponent {
    PromptComponent::from(PromptString::new(
        "»:\n",
        None::<String>,
        AppearingCondition::Default,
        Color::Yellow,
    ))
}

/// Returns the prompt component that prints the local IPV4 address of the
/// machine.
fn create_local_ip_address_component() -> PromptComponent {
    let color: Color = Color::Blue;
    let symbol: PromptString =
        PromptString::new(" ", Some("IP "), AppearingCondition::Default, color);
    let host: String = String::from("%m");
    let separator: PromptString =
        PromptString::new("@", None::<String>, AppearingCondition::Default, color);
    let local_ip_address: String = Network::get_local_ip_address();
    PromptComponent::from(format!(
        "{}{}{}{}",
        symbol, host, separator, local_ip_address
    ))
}

/// Returns the prompt component that prints the disk usage percentage.
fn create_disk_usage_percentage_component() -> PromptComponent {
    let symbol: PromptString = PromptString::new(
        "󰋊 ",
        Some("DIS "),
        AppearingCondition::Default,
        Color::Yellow,
    );
    let usage_percentage: String = format!("{}%%", MainDisk::get_usage_percentage());
    PromptComponent::from(format!("{}{}", symbol, usage_percentage))
}

/// Returns the prompt component that prints the calendar.
fn create_calendar_component() -> PromptComponent {
    let symbol: PromptString =
        PromptString::new("󰸗 ", Some("CAL "), AppearingCondition::Default, Color::Red);
    PromptComponent::from(format!("{}{}", symbol, Calendar::from_current_moment()))
}

/// Returns the prompt component that prints the clock.
fn create_clock_component() -> PromptComponent {
    let clock: Clock = Clock::from_current_moment();
    let symbol: PromptString = match clock.get_day_moment() {
        DayMoment::Dawn => {
            PromptString::new("󰭎 ", Some("DAW "), AppearingCondition::Default, Color::Cyan)
        }
        DayMoment::Morning => {
            PromptString::new("󰖨 ", Some("MOR "), AppearingCondition::Default, Color::Red)
        }
        DayMoment::Afternoon => {
            PromptString::new(" ", Some("AFT "), AppearingCondition::Default, Color::Blue)
        }
        DayMoment::Night => PromptString::new(
            "󰽥 ",
            Some("NIG "),
            AppearingCondition::Default,
            Color::Yellow,
        ),
    };
    PromptComponent::from(format!("{}{}", symbol, clock))
}

/// Returns the prompt component that prints a decorator if the user is `root`.
fn create_root_component() -> PromptComponent {
    let curly_brackets_color: Color = Color::Yellow;
    let left_curly_bracket: PromptString = PromptString::new(
        "{",
        None::<String>,
        AppearingCondition::Default,
        curly_brackets_color,
    );
    let right_curly_bracket: PromptString = PromptString::new(
        "}",
        None::<String>,
        AppearingCondition::Default,
        curly_brackets_color,
    );
    let symbol: PromptString =
        PromptString::new("#", None::<String>, AppearingCondition::Default, Color::Red);
    PromptComponent::from(PromptString::new(
        format!("{}{}{}", left_curly_bracket, symbol, right_curly_bracket),
        None::<String>,
        AppearingCondition::OnRootUser,
        Color::Default,
    ))
}

/// Returns the prompt component that prints different decorators based if the
/// last command succeded or failed.
fn create_error_component() -> PromptComponent {
    let curly_brackets_color: Color = Color::Yellow;
    let left_curly_bracket: PromptString = PromptString::new(
        "{",
        None::<String>,
        AppearingCondition::Default,
        curly_brackets_color,
    );
    let right_curly_bracket: PromptString = PromptString::new(
        "}",
        None::<String>,
        AppearingCondition::Default,
        curly_brackets_color,
    );
    let success_symbol: PromptString =
        PromptString::new("≗", Some("o"), AppearingCondition::OnSuccess, Color::Yellow);
    let error_symbol: PromptString =
        PromptString::new("⨲", Some("*"), AppearingCondition::OnError, Color::Red);
    PromptComponent::from(format!(
        "{}{}{}{}",
        left_curly_bracket, success_symbol, error_symbol, right_curly_bracket
    ))
}

/// Returns the prompt component that prints an arrow decorator.
fn create_arrow_component() -> PromptComponent {
    PromptComponent::from(PromptString::new(
        "⤐  ",
        Some(">%%> "),
        AppearingCondition::Default,
        Color::Yellow,
    ))
}

/// Returns the prompt component that prints sourced virtual environment.
fn create_virtual_environment_component() -> PromptComponent {
    let mut component: PromptComponent = PromptComponent::new();
    if let Some(virtual_environment) = Paths::get_virtual_environment() {
        let curly_brackets_color: Color = Color::Cyan;
        let left_curly_bracket: PromptString = PromptString::new(
            "(",
            None::<String>,
            AppearingCondition::Default,
            curly_brackets_color,
        );
        let right_curly_bracket: PromptString = PromptString::new(
            ") ",
            None::<String>,
            AppearingCondition::Default,
            curly_brackets_color,
        );
        component.push(format!(
            "{}{}{}",
            left_curly_bracket,
            PathTreater::get_base_name(&virtual_environment),
            right_curly_bracket
        ));
    }
    component
}

/// Returns the prompt component that prints the directory path.
fn create_directory_component(repository: &Option<Repository>) -> PromptComponent {
    PromptComponent::from(PromptString::new(
        PathTreater::abbreviate(&Paths::get_current_directory(), repository),
        None::<String>,
        AppearingCondition::Default,
        Color::Red,
    ))
}

/// Returns the prompt component that prints the Git branch.
fn create_git_component(repository: &Option<Repository>) -> PromptComponent {
    let mut component: PromptComponent = PromptComponent::new();
    if let Some(repository) = repository {
        let left_connector: PromptString = PromptString::new(
            ":«",
            None::<String>,
            AppearingCondition::Default,
            Color::Green,
        );
        let right_connector: PromptString = PromptString::new(
            "»",
            None::<String>,
            AppearingCondition::Default,
            Color::Green,
        );
        let branch: String = repository.get_branch().get_name();
        component.push(format!("{}{}{}", left_connector, branch, right_connector));
    }
    component
}

/// Returns the prompt component that prints a decorator whenever the user does
/// not own the current directory.
fn create_directory_ownership_component() -> PromptComponent {
    let mut component: PromptComponent = PromptComponent::new();
    if !PathsPermissions::does_user_owns_current_directory() {
        component.push(PromptString::new(
            " ",
            Some(" #"),
            AppearingCondition::Default,
            Color::Magenta,
        ));
    }
    component
}

/// Returns the prompt component that prints a decorator that marks the position
/// where the user can type commands.
fn create_cursor_component() -> PromptComponent {
    PromptComponent::from(PromptString::new(
        " ⩺",
        Some(" %%>"),
        AppearingCondition::Default,
        Color::Cyan,
    ))
}

fn main() {
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
    prompt.push(create_error_component());
    prompt.push(create_arrow_component());
    prompt.push(create_virtual_environment_component());
    prompt.push(create_directory_component(&repository));
    prompt.push(create_git_component(&repository));
    prompt.push(create_directory_ownership_component());
    prompt.push(create_cursor_component());
    prompt.push(create_horizontal_separator_component());
    print!("{}", prompt);
}
