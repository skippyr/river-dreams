use chrono::
{
	DateTime,
	Local, Datelike
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

pub struct Calendar
{
	day: u32,
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
			day: local_time.day(),
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
			self.day,
			self.year
		)
	}
}

