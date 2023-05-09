use std::env::var;

pub struct EnvironmentVariables;

impl EnvironmentVariables
{
	pub fn is_to_use_fallback_text() -> bool
	{
		match var("RIVER_DREAMS_USE_FALLBACK_TEXT")
		{
			Ok(fallback) =>
			{ fallback == String::from("1") }
			Err(_error) =>
			{ false }
		}
	}

	pub fn get_home() -> String
	{
		match var("HOME")
		{
			Ok(home) =>
			{ home }
			Err(_error) =>
			{ String::new() }
		}
	}

	pub fn get_pwd() -> String
	{
		match var("PWD")
		{
			Ok(pwd) =>
			{ pwd }
			Err(_error) =>
			{ String::from("/") }
		}
	}
}

