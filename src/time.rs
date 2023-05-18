use chrono::
{
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
	pub fn from(moment: DateTime<Local>) -> String
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

	pub fn from(moment: DateTime<Local>) -> String
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

pub struct Calendar
{
	month: String,
	day: String
}

impl Calendar
{
	pub fn from_current_moment() -> Calendar
	{
		let current_moment: DateTime<Local> = Local::now();
		let month: String = Month::from(current_moment);
		let day: String = Day::from(current_moment);
		Calendar
		{
			month,
			day
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
			"{} {}",
			self.month,
			self.day
		)
	}
}

