//! A module that contains functionalities related to time.

use crate::error::Error;
use chrono::{DateTime, Datelike, Local, Timelike};

/// Represents a day ordinal.
pub enum DayOrdinal {
    /// The first ordinal.
    First,
    /// The second ordinal.
    Second,
    /// The third ordinal.
    Third,
    /// The remaining ordinals.
    Other,
}

impl DayOrdinal {
    /// Gets the numeric value that represents an ordinal.
    ///
    /// # Returns
    /// The numeric value that represents an ordinal or nothing.
    pub fn value(&self) -> Option<u32> {
        match self {
            Self::First => Some(1),
            Self::Second => Some(2),
            Self::Third => Some(3),
            Self::Other => None,
        }
    }

    /// Gets the suffix that represents an ordinal.
    ///
    /// # Returns
    /// The suffix that represents an ordinal.
    pub fn suffix(&self) -> &'static str {
        match self {
            Self::First => "st",
            Self::Second => "nd",
            Self::Third => "rd",
            Self::Other => "th",
        }
    }
}

/// Contains the fractions of a day.
pub enum DayFraction {
    /// From 00h00m to 05h59m.
    Dawn,
    /// From 06h00m to 11h59m.
    Morning,
    /// From 12h00m to 17h59m.
    Afternoon,
    /// From 18h00m to 23h59m.
    Night,
}

/// Checks whether the day of a local time corresponds to a certain ordinal.
///
/// # Parameters
/// - `local_time`: the local time to be checked.
/// - `ordinal`: the ordinal to be applied.
///
/// # Returns
/// A boolean that states the day of the local time corresponds to the ordinal or an error.
///
/// # Errors
/// An error happens if the ordinal provided does not has a value associated with it.
fn is_day_ordinal(local_time: &DateTime<Local>, ordinal: DayOrdinal) -> Result<bool, Error> {
    let value: Option<u32> = ordinal.value();
    if value.is_none() {
        Err(Error::new(
            "can not retrieve the value of ordinal for checking local time.",
        ))
    } else {
        Ok((local_time.day() - value.unwrap()) % 10 == 0)
    }
}

/// Gets the day ordinal that coressponds to the day of a local time.
///
/// # Parameters
/// - `local_time`: the local time to be checked.
///
/// # Returns
/// The day ordinal that coressponds to the day of the local time.
pub fn day_ordinal(local_time: &DateTime<Local>) -> DayOrdinal {
    if is_day_ordinal(local_time, DayOrdinal::First).unwrap() {
        DayOrdinal::First
    } else if is_day_ordinal(local_time, DayOrdinal::Second).unwrap() {
        DayOrdinal::Second
    } else if is_day_ordinal(local_time, DayOrdinal::Third).unwrap() {
        DayOrdinal::Third
    } else {
        DayOrdinal::Other
    }
}

/// Gets the fraction of the day that corresponds to a local time.
///
/// # Parameters
/// - `local_time`: the local time to be checked.
///
/// # Returns
/// The fraction of the day that corresponds to the local time.
pub fn day_fraction(local_time: &DateTime<Local>) -> DayFraction {
    match local_time.hour() {
        0..6 => DayFraction::Dawn,
        6..12 => DayFraction::Morning,
        12..18 => DayFraction::Afternoon,
        _ => DayFraction::Night,
    }
}
