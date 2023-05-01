use chrono::{
	DateTime,
	Local,
	Datelike
};

pub fn get_day_of_month_ordinal_suffix(day_of_month: i32) -> String
{
	let is_first: bool = (day_of_month - 1) % 10 == 0;
	let is_second: bool = (day_of_month - 2) % 10 == 0;
	let is_third: bool = (day_of_month - 3) % 10 == 0;
	if is_first
	{ String::from("st") }
	else if is_second
	{ String::from("nd") }
	else if is_third
	{ String::from("rd") }
	else
	{ String::from("th") }
}

pub fn get_calendar_string(local_time: &DateTime<Local>) -> String
{
	local_time
		.format(&format!(
			"(%a) %b %_d{}, %Y",
			get_day_of_month_ordinal_suffix(local_time.day() as i32)
		))
		.to_string()
}

pub fn get_clock_string(local_time: &DateTime<Local>) -> String
{
	local_time
		.format("%Hh%Mm")
		.to_string()
}

