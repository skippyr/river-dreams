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

pub struct Calendar
{
	month: String,
	day: u8
}

impl Calendar
{
	pub fn from_current_moment() -> Calendar
	{
		let current_moment: DateTime<Local> = Local::now();
		let month: String = Month::from(current_moment);
		let day: u8 = current_moment.day() as u8;
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

