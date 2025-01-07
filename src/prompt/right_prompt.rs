//! A module that contains the functionalities related to the right prompt of the theme.

use super::Prompt;
use crate::{
    error::Error,
    file_system::DirectoryEntryTotals,
    shell::{Color, Zsh},
};
use num_format::{Locale, ToFormattedString};

/// Represents the right prompt of the theme.
pub struct RightPrompt;

impl RightPrompt {
    /// Writes a total number of directory entries and its colored symbol if the total is greater than zero.
    ///
    /// # Parameters
    /// - `total`: the total number of directory entries.
    /// - `symbol`: the symbol to be used.
    /// - `color`: the color to be applied or nothing.
    fn write_entry_total_symbol(total: usize, symbol: &'static str, color: Option<Color>) {
        if total > 0 {
            print!(
                " {}{}",
                Zsh::color_symbol(symbol, color),
                total.to_formatted_string(&Locale::en)
            );
        }
    }

    /// Writes the directory summary module which displays the total number of each entry type in the current directory.
    ///
    /// # Parameters
    /// - `entry_totals`: the total of each entry type in the current directory.
    fn write_directory_summary_module(entry_totals: &DirectoryEntryTotals) {
        Self::write_entry_total_symbol(entry_totals.total_directories(), " ", Some(Color::Yellow));
        Self::write_entry_total_symbol(entry_totals.total_files(), " ", None);
        Self::write_entry_total_symbol(entry_totals.total_sockets(), "󱄙 ", Some(Color::Cyan));
        Self::write_entry_total_symbol(entry_totals.total_fifos(), "󰟦 ", Some(Color::Blue));
        Self::write_entry_total_symbol(entry_totals.total_blocks(), "󰇖 ", Some(Color::Magenta));
        Self::write_entry_total_symbol(entry_totals.total_characters(), "󱣴 ", Some(Color::Green));
        Self::write_entry_total_symbol(entry_totals.total_symlinks(), "󰌷 ", Some(Color::Blue));
        Self::write_entry_total_symbol(entry_totals.total_hiddens(), "󰈉 ", Some(Color::Red));
        Self::write_entry_total_symbol(
            entry_totals.total_temporaries(),
            "󱣹 ",
            Some(Color::Magenta),
        );
    }

    /// Writes the jobs modules which displays the total number of background jobs running in the shell.
    fn write_jobs_modules() {
        print!(
            "{}",
            Zsh::show_symbol_when_job(format!(
                " {} {}",
                Zsh::color_symbol("", Some(Color::Magenta)),
                Zsh::TOTAL_JOBS
            ))
        );
    }
}

impl Prompt for RightPrompt {
    /// Writes the right prompt of the theme.
    ///
    /// # Returns
    /// A result that may contain a possible error encountered.
    ///
    /// # Errors
    /// An error happens if the current directory can not be accessed.
    fn write() -> Result<(), Error> {
        let entry_totals: DirectoryEntryTotals = DirectoryEntryTotals::from_current_directory()?;
        Self::write_directory_summary_module(&entry_totals);
        Self::write_jobs_modules();
        println!();
        Ok(())
    }
}
