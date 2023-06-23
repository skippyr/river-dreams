//! A module to work with formatting.

use num_format::{Locale, ToFormattedString};

const LOCALE: Locale = Locale::en;

/// An abstraction to format numbers.
pub struct NumberFormatter;

impl NumberFormatter {
    /// Formats a number as a human readable string containing commas to easy
    /// reading.
    pub fn format_u32(number: u32) -> String {
        number.to_formatted_string(&LOCALE)
    }
}
