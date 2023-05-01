use chrono::{
	DateTime,
	Local,
	Timelike
};
use super::{
	environment_variables::is_to_use_fallback_text,
	styles::{
		colorize_string,
		Color
	}
};

pub struct Symbol
{
	pub default: String,
	pub fallback: String
}

pub fn get_symbol_string(symbol: Symbol) -> String
{
	if is_to_use_fallback_text()
	{ symbol.fallback }
	else
	{ symbol.default }
}

pub fn get_colorized_clock_symbol(local_time: &DateTime<Local>) -> String
{
	if is_to_use_fallback_text()
	{
		colorize_string(
			String::from("Clock"),
			Color::Yellow
		)
	}
	else
	{
		let hour: u32 = local_time.hour();
		let is_dawn: bool = hour < 6;
		let is_morning: bool = !is_dawn && hour < 12;
		let is_afternoon: bool = !is_morning && hour < 18;
		if is_dawn
		{
			colorize_string(
				String::from(" "),
				Color::Cyan
			)
		}
		else if is_morning
		{
			colorize_string(
				String::from("盛"),
				Color::Yellow
			)
		}
		else if is_afternoon
		{
			colorize_string(
				String::from(" "),
				Color::Blue
			)
		}
		else
		{
			colorize_string(
				String::from("󰽥 "),
				Color::Yellow
			)	
		}
	}
}

