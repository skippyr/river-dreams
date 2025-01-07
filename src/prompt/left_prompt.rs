//! A module that contains the functionalities related to the left prompt of the theme.

use super::Prompt;
use crate::{
    error::Error,
    file_system::{can_write_to_current_directory, is_file_system_root},
    format::calculate_total_digits,
    git::{GitReference, GitRepository},
    hardware::{
        battery::{Battery, BatteryCharge, BatteryChargeStatus},
        disk::{Disk, DiskUsage, DiskUsageStatus},
    },
    shell::{Color, Zsh},
    time::{day_fraction, day_ordinal, DayFraction},
};
use chrono::{DateTime, Local};
use crossterm::terminal::size as terminal_size;
use local_ip_address::local_ip;
use std::{
    env::current_dir,
    net::IpAddr,
    path::{Path, PathBuf},
};

/// The constant length of all the modules in the prompt.
const MODULES_CONST_LENGTH: u16 = 42;

/// Represents the left prompt of the theme.
pub struct LeftPrompt;

impl LeftPrompt {
    /// Writes the separator that divides the output of the previous command inserted in the shell from the theme, apearring at its top.
    ///
    /// # Parameters
    /// - `total_columns`: the total number of columns in the terminal.
    fn write_top_separator(total_columns: u16) {
        for column in 0..total_columns {
            print!(
                "{}",
                if column % 2 == 0 {
                    Zsh::color_symbol("≥", Some(Color::Yellow))
                } else {
                    Zsh::color_symbol("v", Some(Color::Red))
                }
            );
        }
        print!("{}", Zsh::color_symbol(":«(", Some(Color::Yellow)));
    }

    /// Writes the ip address module which displays the IP address of the computer.
    ///
    /// # Parameters
    /// - `ip_address`: the ip address of the computer or nothing.
    /// - `modules_length`: the length being occupied by the modules of the prompt.
    fn write_ip_address_module(ip_address: Option<&IpAddr>, modules_length: &mut u16) {
        let ip_address: String = match ip_address {
            Some(ip_address) => ip_address.to_string(),
            None => String::from("No Address Found"),
        };
        print!(
            "{} {}",
            Zsh::color_symbol(" ", Some(Color::Blue)),
            ip_address
        );
        *modules_length += ip_address.len() as u16;
    }

    /// Writes the battery module which, in case of a laptop, displays the information about the charge of the battery of the computer, including its percentage and status.
    ///
    /// # Parameters
    /// - `charge`: the charge information of the computer battery.
    /// - `modules_length`: the length being occupied by the modules of the prompt.
    fn write_battery_module(charge: Option<&BatteryCharge>, modules_length: &mut u16) {
        if charge.is_none() {
            return;
        }
        let charge: &BatteryCharge = charge.unwrap();
        print!(
            "  {} {}{}",
            match charge.status() {
                BatteryChargeStatus::Critical => Zsh::color_symbol(
                    if charge.is_charging() { "󰢟" } else { "󰂎" },
                    Some(Color::Red)
                ),
                BatteryChargeStatus::Low => Zsh::color_symbol(
                    if charge.is_charging() { "󱊤" } else { "󱊡" },
                    Some(Color::Red)
                ),
                BatteryChargeStatus::Moderate => Zsh::color_symbol(
                    if charge.is_charging() { "󱊥" } else { "󱊢" },
                    Some(Color::Yellow)
                ),
                BatteryChargeStatus::High => Zsh::color_symbol(
                    if charge.is_charging() { "󱊦" } else { "󱊣" },
                    Some(Color::Green)
                ),
            },
            charge.percentage(),
            Zsh::PERCENTAGE_SYMBOL
        );
        *modules_length += calculate_total_digits(charge.percentage() as u16) + 5;
    }

    /// Writes the disk module which displays the information about the usage of the main disk of the computer, including its percentage and status.
    ///
    /// # Parameters
    /// - `usage`: the information about the usage of the main disk of the computer.
    /// - `modules_length`: the length being occupied by the modules of the prompt.
    fn write_disk_module(usage: &DiskUsage, modules_length: &mut u16) {
        print!(
            "  {}{}%%",
            Zsh::color_symbol(
                "󰋊 ",
                match usage.status() {
                    DiskUsageStatus::Low => Some(Color::Green),
                    DiskUsageStatus::Moderate => Some(Color::Yellow),
                    DiskUsageStatus::High => Some(Color::Red),
                }
            ),
            usage.percentage()
        );
        *modules_length += calculate_total_digits(usage.percentage() as u16);
    }

    /// Writes the calendar module which displays a calendar showing the week day, month, and day of month.
    ///
    /// # Parameters
    /// - `local_time`: the local time of the computer.
    fn write_calendar_module(local_time: &DateTime<Local>) {
        print!(
            "  {}{}{}",
            Zsh::color_symbol("󰃭 ", Some(Color::Red)),
            local_time.format("(%a) %b %d"),
            day_ordinal(local_time).suffix()
        );
    }

    /// Writes the clock module which display a 24-hours clock with a symbol that changes accordingly to the day fraction.
    ///
    /// # Parameters
    /// - `local_time`: the local time of the computer.
    fn write_clock_module(local_time: &DateTime<Local>) {
        print!(
            "  {}{}",
            match day_fraction(local_time) {
                DayFraction::Dawn => Zsh::color_symbol("󰭎 ", Some(Color::Cyan)),
                DayFraction::Morning => Zsh::color_symbol("󰖨 ", Some(Color::Red)),
                DayFraction::Afternoon => Zsh::color_symbol(" ", Some(Color::Blue)),
                DayFraction::Night => Zsh::color_symbol("󰽥 ", Some(Color::Yellow)),
            },
            local_time.format("%Hh%Mm")
        );
    }

    /// Writes the separator that appears in the middle of the prompt, after the modules.
    ///
    /// # Parameters
    /// - `total_columns`: the total number of columns in the terminal.
    /// - `modules_length`: the length being occupied by the modules of the prompt.
    fn write_middle_separator(total_columns: u16, modules_length: u16) {
        print!("{}", Zsh::color_symbol(")»:", Some(Color::Yellow)));
        for column in 0..if total_columns >= modules_length {
            total_columns - modules_length
        } else {
            0
        } {
            print!(
                "{}",
                if column % 2 == 0 {
                    Zsh::color_symbol("-", Some(Color::Red))
                } else {
                    Zsh::color_symbol("=", Some(Color::Yellow))
                }
            );
        }
    }

    /// Writes the root module which displays a hash symbol when the user is `root`.
    fn write_root_module() {
        print!(
            "{}",
            Zsh::show_symbol_when_root(format!(
                "{}{}{}",
                Zsh::color_symbol("{", Some(Color::Yellow)),
                Zsh::color_symbol("#", Some(Color::Red)),
                Zsh::color_symbol("}", Some(Color::Yellow)),
            ))
        );
    }

    /// Writes the exit code module which displays the exit code of the last command inserted in the shell.
    fn write_exit_code_module() {
        print!(
            "{}{}{}",
            Zsh::color_symbol("{", Some(Color::Yellow)),
            Zsh::show_symbols_in_exit_codes(
                Zsh::color_symbol(Zsh::EXIT_CODE, Some(Color::Yellow)),
                Zsh::color_symbol(Zsh::EXIT_CODE, Some(Color::Red))
            ),
            Zsh::color_symbol("}⤐ ", Some(Color::Yellow))
        );
    }

    /// Writes the virtual env module which displays the active Python virtual environment sourced.
    ///
    /// # Parameters
    /// - `virtual_env`: the active virtual environment or nothing.
    fn write_virtual_env_module(virtual_env: Option<&String>) {
        if let Some(virtual_env) = virtual_env {
            print!(
                " ({})",
                Path::new(virtual_env)
                    .file_name()
                    .unwrap()
                    .to_string_lossy()
            );
        }
    }

    /// Writes the path module which displays the current directory path, abbreviating it when inside of Git repositories.
    ///
    /// # Parameters
    /// - `current_directory`: the path to the current directory.
    /// - `git_repository`: the Git repository active in the current directory or nothing.
    fn write_path_module(current_directory: &PathBuf, git_repository: Option<&GitRepository>) {
        print!(" ");
        if git_repository.is_some() && !is_file_system_root(&git_repository.unwrap().path()) {
            print!(
                "{}",
                Zsh::color_symbol(
                    format!(
                        "@/{}",
                        current_directory
                            .strip_prefix(git_repository.unwrap().path().parent().unwrap())
                            .unwrap()
                            .display()
                    ),
                    Some(Color::Red)
                )
            );
        } else {
            print!(
                "{}",
                Zsh::color_symbol(Zsh::CURRENT_DIRECTORY, Some(Color::Red))
            );
        }
    }

    /// Writes the Git module which displays information about an active repository, including the active reference branch name or hash and a ballot X symbol when there are changes to be commited.
    ///
    /// # Parameters
    /// - `repository`: the Git repository active in the current directory or nothing.
    fn write_git_module(repository: Option<&GitRepository>) {
        if repository.is_none() {
            return;
        }
        let repository: &GitRepository = repository.unwrap();
        print!("{}", Zsh::color_symbol(":«(", Some(Color::Yellow)));
        if let GitReference::Hash(_) = repository.reference() {
            print!("{}:", Zsh::color_symbol("@rebase", Some(Color::Magenta)));
        }
        print!(
            "{}{}",
            match repository.reference() {
                GitReference::Branch(branch) => branch,
                GitReference::Hash(hash) => hash,
            },
            Zsh::color_symbol(")»", Some(Color::Yellow))
        );
        if repository.has_changes() {
            print!(" {}", Zsh::color_symbol("✗ ", Some(Color::Cyan)));
        }
    }

    /// Writes the directory access module which displays a lock symbol whenever the user can not write to the current directory.
    fn write_directory_access_module() {
        if !can_write_to_current_directory() {
            print!(" {}", Zsh::color_symbol("", Some(Color::Cyan)));
        }
    }
}

impl Prompt for LeftPrompt {
    /// Writes the left prompt of the theme.
    ///
    /// # Returns
    /// A result that may contain a possible error encountered.
    ///
    /// # Errors
    /// An error happens if:
    /// - The terminal dimensions can not be retrieved.
    /// - The usage information of the disk of the computer can not be retrieved.
    /// - The charge information of the battery of the computer can not be retrieved.
    /// - The current directory can not be accessed.
    fn write() -> Result<(), Error> {
        let total_columns: u16 = terminal_size()
            .map(|(total_columns, _)| total_columns)
            .map_err(|_| Error::new("can not retrieve the dimensions of the terminal."))?;
        let disk_usage: DiskUsage = Disk::usage()?;
        let battery_charge: Option<BatteryCharge> = Battery::charge()?;
        let local_time: DateTime<Local> = Local::now();
        let virtual_env: Option<String> = std::env::var("VIRTUAL_ENV").ok();
        let current_directory: PathBuf =
            current_dir().map_err(|_| Error::new("can not access the current directory."))?;
        let git_repository: Option<GitRepository> = GitRepository::search_from_current_directory();
        let mut modules_length: u16 = MODULES_CONST_LENGTH;
        Self::write_top_separator(total_columns);
        Self::write_ip_address_module(local_ip().ok().as_ref(), &mut modules_length);
        Self::write_battery_module(battery_charge.as_ref(), &mut modules_length);
        Self::write_disk_module(&disk_usage, &mut modules_length);
        Self::write_calendar_module(&local_time);
        Self::write_clock_module(&local_time);
        Self::write_middle_separator(total_columns, modules_length);
        Self::write_root_module();
        Self::write_exit_code_module();
        Self::write_virtual_env_module(virtual_env.as_ref());
        Self::write_path_module(&current_directory, git_repository.as_ref());
        Self::write_git_module(git_repository.as_ref());
        Self::write_directory_access_module();
        println!(" ");
        Ok(())
    }
}
