use chrono::
{
	self,
	DateTime,
	Local,
	Datelike
};
use std::fmt::
{
	Display,
	Formatter,
	Result
};
use crate::math::Math;

struct Month;

impl Month
{
	fn as_string(moment: DateTime<Local>) -> String
	{
		match moment.month0()
		{
			1 =>
			{ String::from("Feb") }
			2 =>
			{ String::from("Mar") }
			3 =>
			{ String::from("Apr") }
			4 =>
			{ String::from("May") }
			5 =>
			{ String::from("Jun") }
			6 =>
			{ String::from("Jul") }
			7 =>
			{ String::from("Aug") }
			8 =>
			{ String::from("Sep") }
			9 =>
			{ String::from("Oct") }
			10 =>
			{ String::from("Nov") }
			_ =>
			{ String::from("Jan") }
		}
	}
}

struct Day;

impl Day
{
	fn get_ordinal_string(day: u8) -> String
	{
		if Math::is_first(day)
		{ String::from("st") }
		else if Math::is_second(day)
		{ String::from("nd") }
		else if Math::is_third(day)
		{ String::from("rd") }
		else
		{ String::from("th") }
	}

	fn as_string(moment: DateTime<Local>) -> String
	{
		let day: u8 = moment.day() as u8;
		let ordinal: String = Self::get_ordinal_string(day);
		format!(
			"{}{}",
			day,
			ordinal
		)
	}
}

struct WeekDay;

impl WeekDay
{
	pub fn as_string(moment: DateTime<Local>) -> String
	{
		match moment.weekday()
		{
			chrono::Weekday::Sun =>
			{ String::from("Sun") }
			chrono::Weekday::Mon =>
			{ String::from("Mon") }
			chrono::Weekday::Tue =>
			{ String::from("Tue") }
			chrono::Weekday::Wed =>
			{ String::from("Wed") }
			chrono::Weekday::Thu =>
			{ String::from("Thu") }
			chrono::Weekday::Fri =>
			{ String::from("Fri") }
			chrono::Weekday::Sat =>
			{ String::from("Sat") }
		}
	}
}

pub struct Calendar
{
	month: String,
	day: String,
	week_day: String
}

impl Calendar
{
	pub fn from_current_moment() -> Calendar
	{
		let current_moment: DateTime<Local> = Local::now();
		let month: String = Month::as_string(current_moment);
		let day: String = Day::as_string(current_moment);
		let week_day: String = WeekDay::as_string(current_moment);
		Calendar
		{
			month,
			day,
			week_day
		}
	}
}

impl Display for Calendar
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		write!(
			formatter,
			"({}) {} {}",
			self.week_day,
			self.month,
			self.day
		)
	}
}

