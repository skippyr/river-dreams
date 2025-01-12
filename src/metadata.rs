//! A module that contains metadata and functionalities related to metadata of the theme.

use crate::{hardware::cpu::Cpu, operating_system::OperatingSystem};
use crossterm::style::Stylize;

/// Represents the metadata of the theme.
pub struct ThemeMetadata;

impl ThemeMetadata {
    /// Gets the name of the theme.
    ///
    /// # Returns
    /// The name of the theme.
    pub fn name() -> &'static str {
        "river_dreams"
    }

    /// Gets the version of the theme.
    ///
    /// # Returns
    /// The version of the theme.
    pub fn version() -> &'static str {
        env!("CARGO_PKG_VERSION")
    }

    /// Gets the URL of the repository that contains the theme.
    ///
    /// # Returns
    /// The URL of the repository that contains the theme.
    pub fn repository_url() -> &'static str {
        "https://github.com/skippyr/river_dreams"
    }
}

/// Writes the theme help instructions shown when the flags `-h` and `--help` are used.
pub fn write_help() {
    println!(
        "{} {} <{}> [{}]...",
        "Usage:".dark_magenta().bold(),
        ThemeMetadata::name().dark_yellow(),
        "SIDE".underlined().dark_cyan(),
        "OPTIONS".underlined().dark_cyan()
    );
    println!("Writes a side portion of the shell theme.");
    println!();
    println!("{}", "❡ SHELL INTEGRATION".dark_magenta().bold());
    println!("For integrating it with your shell, write its connector to your ZSH");
    println!("configuration file.");
    println!();
    println!(
        "    {} --connector >> ~/.zshrc;",
        ThemeMetadata::name().dark_yellow()
    );
    println!();
    println!("Then, reopen your shell.");
    println!();
    println!("{}", "❡ AVAILABLE SIDES".dark_magenta().bold());
    println!(
        "    {}, {}   refers to the left prompt.",
        "l".dark_red(),
        "left".dark_red()
    );
    println!(
        "    {}, {}  refers to the right prompt.",
        "r".dark_red(),
        "right".dark_red()
    );
    println!();
    println!("{}", "❡ AVAILABLE OPTIONS".dark_magenta().bold());
    println!(
        "    {}, {}     writes the software help instructions.",
        "-h".dark_red(),
        "--help".dark_red()
    );
    println!(
        "    {}, {}  writes the software version.",
        "-v".dark_red(),
        "--version".dark_red()
    );
    println!(
        "    {}    writes the theme connector.",
        "--connector".dark_red()
    );
}

/// Writes the theme version shown when the flags `-v` and `--version` are used.
pub fn write_version() {
    println!(
        "{} {} {}",
        ThemeMetadata::name(),
        format!("v{}", ThemeMetadata::version()).dark_green(),
        format!(
            "(compiled for {} {})",
            OperatingSystem::name(),
            Cpu::architecture_name()
        )
        .dark_grey()
    );
    println!(
        "Available at: {}.",
        ThemeMetadata::repository_url().dark_cyan().underlined()
    );
}

/// Writes the connector of the theme, a script that simply makes its output be set as the shell prompts.
pub fn write_connector() {
    println!("setopt promptsubst;");
    println!("export VIRTUAL_ENV_DISABLE_PROMPT=1;");
    println!("PROMPT='$(river_dreams l)';");
    println!("RPROMPT='$(river_dreams r)';");
}
