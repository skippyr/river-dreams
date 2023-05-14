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

	pub fn get_virtual_env() -> Option<String>
	{
		match var("VIRTUAL_ENV")
		{
			Ok(virtual_env) =>
			{ Some(virtual_env) }
			Err(_error) =>
			{ None }
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
}

