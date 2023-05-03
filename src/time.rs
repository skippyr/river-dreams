use chrono::
{
	DateTime,
	Local,
	Datelike,
	Timelike
};

pub struct Month
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

pub struct WeekDay
{ value: u32 }

impl WeekDay
{
	pub fn from(value: u32) -> WeekDay
	{ WeekDay { value } }

	pub fn as_abbreviated_string(&self) -> Option<String>
	{
		match self.value
		{
			0 =>
			{ Some(String::from("Sun")) }
			1 =>
			{ Some(String::from("Mon")) }
			2 =>
			{ Some(String::from("Tue")) }
			3 =>
			{ Some(String::from("Wed")) }
			4 =>
			{ Some(String::from("Thu")) }
			5 =>
			{ Some(String::from("Fri")) }
			6 =>
			{ Some(String::from("Sat")) }
			_ =>
			{ None }
		}
	}
}

pub struct Day
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
	month: Month,
	week_day: WeekDay,
	year: i32
}

impl Calendar
{
	pub fn from_current_moment() -> Calendar
	{
		let local_time: DateTime<Local> = Local::now();
		Calendar
		{
			day: Day::from(local_time.day()),
			month: Month::from(local_time.month()),
			week_day: WeekDay::from(local_time.weekday().num_days_from_sunday()),
			year: local_time.year()
		}
	}

	pub fn as_string(&self) -> String
	{
		let month_as_abbreviated_string: String = match self.month.as_abbreviated_string()
		{
			Some(month_as_abbreviated_string) =>
			{ month_as_abbreviated_string }
			None =>
			{ String::new() }
		};
		let week_day_as_abbreviated_string: String = match self.week_day.as_abbreviated_string()
		{
			Some(week_day_as_abbreviated_string) =>
			{ week_day_as_abbreviated_string }
			None =>
			{ String::new() } 
		};
		format!(
			"({}) {} {}, {}",
			week_day_as_abbreviated_string,
			month_as_abbreviated_string,
			self.day.as_ordinal_string(),
			self.year
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

	pub fn as_string(&self) -> String
	{
		format!(
			"{}h{}m",
			self.hour,
			self.minute
		)
	}
}

