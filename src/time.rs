use chrono::
{
	DateTime,
	Local,
	Datelike,
	Timelike
};
use std::fmt::
{
	Display,
	Formatter,
	Result
};

struct Month
{ value: u32 }

impl Month
{
	pub fn from(value: u32) -> Month
	{ Month { value } }

	pub fn as_abbreviated_string(&self) -> Option<String>
	{
		match self.value
		{
			1 =>
			{ Some(String::from("Jan")) }
			2 =>
			{ Some(String::from("Feb")) }
			3 =>
			{ Some(String::from("Mar")) }
			4 =>
			{ Some(String::from("Apr")) }
			5 =>
			{ Some(String::from("May")) }
			6 =>
			{ Some(String::from("Jun")) }
			7 =>
			{ Some(String::from("Jul")) }
			8 =>
			{ Some(String::from("Aug")) }
			9 =>
			{ Some(String::from("Sep")) }
			10 =>
			{ Some(String::from("Oct")) }
			11 =>
			{ Some(String::from("Nov")) }
			12 =>
			{ Some(String::from("Dec")) }
			_ =>
			{ None }
		}
	}
}

struct Day
{ value: u32 }

impl Day
{
	pub fn from(value: u32) -> Day
	{ Day { value } }

	pub fn as_ordinal_string(&self) -> String
	{
		let value: i8 = self.value as i8;
		let is_first: bool = (value - 1) % 10 == 0;
		let is_second: bool = (value - 2) % 10 == 0;
		let is_third: bool = (value - 3) % 10 == 0;
		if is_first
		{
			format!(
				"{}st",
				value
			)
		}
		else if is_second
		{
			format!(
				"{}nd",
				value
			)
		}
		else if is_third
		{
			format!(
				"{}rd",
				value
			)
		}
		else
		{
			format!(
				"{}th",
				value
			)
		}
	}
}

pub struct Calendar
{
	day: Day,
	month: Month
}

impl Calendar
{
	pub fn from_current_moment() -> Calendar
	{
		let local_time: DateTime<Local> = Local::now();
		Calendar
		{
			day: Day::from(local_time.day()),
			month: Month::from(local_time.month())
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
		let month_as_abbreviated_string: String = match self.month.as_abbreviated_string()
		{
			Some(month_as_abbreviated_string) =>
			{ month_as_abbreviated_string }
			None =>
			{ String::new() }
		};
		write!(
			formatter,
			"{} {}",
			month_as_abbreviated_string,
			self.day.as_ordinal_string()
		)  
	}
}

pub enum DayMoment
{
	Dawn,
	Morning,
	Afternoon,
	Night
}

pub struct Clock
{
	hour: u32,
	minute: u32
}

impl Clock
{
	pub fn from_current_moment() -> Clock
	{
		let local_time: DateTime<Local> = Local::now();
		Clock
		{
			hour: local_time.hour(),
			minute: local_time.minute()
		}
	}

	pub fn get_day_moment(&self) -> DayMoment
	{
		let is_dawn: bool = self.hour < 6;
		let is_morning: bool = !is_dawn && self.hour < 12;
		let is_afternoon: bool = !is_morning && self.hour < 18;
		if is_dawn
		{ DayMoment::Dawn }
		else if is_morning
		{ DayMoment::Morning }
		else if is_afternoon
		{ DayMoment::Afternoon }
		else
		{ DayMoment::Night }
	}
}

impl Display for Clock
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		write!(
			formatter,
			"{}{}h{}{}m",
			if self.hour < 10
			{ String::from("0") }
			else
			{ String::new() },
			self.hour,
			if self.minute < 10
			{ String::from("0") }
			else
			{ String::new() },
			self.minute
		)
	}
}

