use chrono::{
	DateTime,
	Local,
	Timelike
};
use crate::time::{DayMoment, get_day_moment};

use super::{
	environment_variables::is_to_use_fallback_text,
	styles::Color
};

pub struct Symbol
{
	pub default: String,
	pub fallback: String,
	pub color: Color
}

pub fn choose_symbol_string_for_environment(symbol: &Symbol) -> String
{
	if is_to_use_fallback_text()
	{ symbol.fallback.clone() }
	else
	{ symbol.default.clone() }
}

pub fn get_clock_symbol(local_time: &DateTime<Local>) -> Symbol
{
	let fallback: String = String::from("Clock ");
	match get_day_moment(local_time.hour())
	{
		DayMoment::Dawn =>
		{
			Symbol
			{
				default: String::from(" "),
				fallback,
				color: Color::Cyan
			}
		}
		DayMoment::Morning =>
		{
			Symbol
			{
				default: String::from("盛"),
				fallback,
				color: Color::Red
			}
		}
		DayMoment::Afternoon =>
		{
			Symbol
			{
				default: String::from(" "),
				fallback,
				color: Color::Blue
			}
		}
		DayMoment::Night =>
		{
			Symbol
			{
				default: String::from("󰽥 "),
				fallback,
				color: Color::Yellow
			}
		}
	}
}

