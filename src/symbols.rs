use super::{
	time::{
		DayMoment,
		get_day_moment
	},
	environment_variables::is_to_use_fallback_text,
	math::is_pair,
	styles::Color
};
use chrono::{
	DateTime,
	Local,
	Timelike
};

pub struct Symbol
{
	pub default_text: String,
	pub fallback_text: String,
	pub color: Color
}

impl Symbol
{
	pub fn get_text_for_environment(&self) -> String
	{
		if is_to_use_fallback_text()
		{ self.fallback_text.clone() }
		else
		{ self.default_text.clone() }
	}
}

pub fn get_commands_separator_symbol(column: u16) -> Symbol
{
	if is_pair(column)
	{
		Symbol
		{
			default_text: String::from(""),
			fallback_text: String::from("="),
			color: Color::Red
		}
	}
	else
	{
		Symbol
		{
			default_text: String::from(""),
			fallback_text: String::from("-"),
			color: Color::Normal
		}
	}
}

pub fn get_clock_symbol(local_time: &DateTime<Local>) -> Symbol
{
	let fallback_text: String = String::from("Clock ");
	match get_day_moment(local_time.hour())
	{
		DayMoment::Dawn =>
		{
			Symbol
			{
				default_text: String::from(" "),
				fallback_text,
				color: Color::Cyan
			}
		}
		DayMoment::Morning =>
		{
			Symbol
			{
				default_text: String::from("盛"),
				fallback_text,
				color: Color::Red
			}
		}
		DayMoment::Afternoon =>
		{
			Symbol
			{
				default_text: String::from(" "),
				fallback_text,
				color: Color::Blue
			}
		}
		DayMoment::Night =>
		{
			Symbol
			{
				default_text: String::from("󰽥 "),
				fallback_text,
				color: Color::Yellow
			}
		}
	}
}

