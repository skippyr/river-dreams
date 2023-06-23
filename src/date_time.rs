//! A module to work with date and time operations.

use crate::math::Math;
use chrono::{self, DateTime, Datelike, Local, Timelike};
use std::fmt::{Display, Formatter, Result};

/// An abstraction of a month.
struct Month;

impl Month {
    /// Returns the name of the month abbreviated.
    fn as_string(moment: DateTime<Local>) -> String {
        match moment.month0() {
            1 => String::from("Feb"),
            2 => String::from("Mar"),
            3 => String::from("Apr"),
            4 => String::from("May"),
            5 => String::from("Jun"),
            6 => String::from("Jul"),
            7 => String::from("Aug"),
            8 => String::from("Sep"),
            9 => String::from("Oct"),
            10 => String::from("Nov"),
            _ => String::from("Jan"),
        }
    }
}

/// An abstraction of a day.
struct Day;

impl Day {
    /// Returns the ordinal the day applies on.
    fn get_ordinal_string(day: u8) -> String {
        if Math::is_first(day) {
            String::from("st")
        } else if Math::is_second(day) {
            String::from("nd")
        } else if Math::is_third(day) {
            String::from("rd")
        } else {
            String::from("th")
        }
    }

    /// Returns the day as a ordinal string.
    fn as_string(moment: DateTime<Local>) -> String {
        let day: u8 = moment.day() as u8;
        let ordinal: String = Self::get_ordinal_string(day);
        format!("{}{}", day, ordinal)
    }
}

/// An abstraction of a day of the week.
struct WeekDay;

impl WeekDay {
    /// Returns the name of the day of the week abbreviated.
    pub fn as_string(moment: DateTime<Local>) -> String {
        match moment.weekday() {
            chrono::Weekday::Sun => String::from("Sun"),
            chrono::Weekday::Mon => String::from("Mon"),
            chrono::Weekday::Tue => String::from("Tue"),
            chrono::Weekday::Wed => String::from("Wed"),
            chrono::Weekday::Thu => String::from("Thu"),
            chrono::Weekday::Fri => String::from("Fri"),
            chrono::Weekday::Sat => String::from("Sat"),
        }
    }
}

/// An abstraction to work with dates.
pub struct Calendar {
    month: String,
    day: String,
    week_day: String,
}

impl Calendar {
    /// Returns an instance of a `Calendar` based on the current date values.
    pub fn from_current_moment() -> Self {
        let current_moment: DateTime<Local> = Local::now();
        let month: String = Month::as_string(current_moment);
        let day: String = Day::as_string(current_moment);
        let week_day: String = WeekDay::as_string(current_moment);
        Self {
            month,
            day,
            week_day,
        }
    }
}

impl Display for Calendar {
    fn fmt(&self, formatter: &mut Formatter) -> Result {
        write!(formatter, "({}) {} {}", self.week_day, self.month, self.day)
    }
}

/// A set containing all moments of a day.
pub enum DayMoment {
    /// The moment from 6 to 12 hours.
    Morning,
    /// The moment from 12 to 18 hours.
    Afternoon,
    /// The moment from 18 to 0 hours.
    Night,
    /// The moment from 0 to 6 hours.
    Dawn,
}

/// An abstraction to work with time.
pub struct Clock {
    hours: i8,
    minutes: i8,
}

impl Clock {
    /// Format an hour or minute as a two digits string. If the value is only
    /// one digit, a zero will be added in its left side.
    fn format_time(time: i8) -> String {
        format!(
            "{}{}",
            if time < 10 {
                String::from("0")
            } else {
                String::new()
            },
            time
        )
    }

    /// Creates an instance of `Clock` based on the current time values.
    pub fn from_current_moment() -> Self {
        let current_moment: DateTime<Local> = Local::now();
        let hours: i8 = current_moment.hour() as i8;
        let minutes: i8 = current_moment.minute() as i8;
        Self { hours, minutes }
    }

    /// Checks if the time is within the limits of the `DayMoment::Dawn` day
    /// moment. It returns `true` if it is and `false` otherwise.
    fn is_dawn(&self) -> bool {
        self.hours >= 0 && self.hours < 6
    }

    /// Checks if the time is within the limits of the `DayMoment::Morning` day
    /// moment. It returns `true` if it is and `false` otherwise.
    fn is_morning(&self) -> bool {
        self.hours >= 6 && self.hours < 12
    }

    /// Checks if the time is within the limits of the `DayMoment::Afternoon`
    /// day moment. It returns `true` if it is and `false` otherwise.
    fn is_afternoon(&self) -> bool {
        self.hours >= 12 && self.hours < 18
    }

    /// Returns the day moment the time applies on.
    pub fn get_day_moment(&self) -> DayMoment {
        if self.is_dawn() {
            DayMoment::Dawn
        } else if self.is_morning() {
            DayMoment::Morning
        } else if self.is_afternoon() {
            DayMoment::Afternoon
        } else {
            DayMoment::Night
        }
    }
}

impl Display for Clock {
    fn fmt(&self, formatter: &mut Formatter) -> Result {
        write!(
            formatter,
            "{}h{}m",
            Self::format_time(self.hours),
            Self::format_time(self.minutes)
        )
    }
}
