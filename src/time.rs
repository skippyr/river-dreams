use chrono::
{
	DateTime,
	Local,
	Datelike
};

pub enum DayMoment
{
	Dawn,
	Morning,
	Afternoon,
	Night
}

pub fn get_day_moment(hour_in_24h_format: u32) -> DayMoment
{
	let is_dawn: bool = hour_in_24h_format < 6;
	let is_morning: bool = !is_dawn && hour_in_24h_format < 12;
	let is_afternoon: bool = !is_morning && hour_in_24h_format < 18;
	if is_dawn
	{ DayMoment::Dawn }
	else if is_morning
	{ DayMoment::Morning }
	else if is_afternoon
	{ DayMoment::Afternoon }
	else
	{ DayMoment::Night }
}

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
{ local_time.format("%Hh%Mm").to_string() }

