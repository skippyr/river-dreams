use std::
{
	process::exit,
	env::var
};
use super::error_treatment::print_error;

pub fn is_to_use_fallback_text() -> bool
{
	match var("RIVER_DREAMS_USE_FALLBACK_TEXT")
	{
		Ok(is_to_use_fallback_text) =>
		{ is_to_use_fallback_text == "1" }
		Err(_error) =>
		{ false }
	}
}

pub fn get_pwd() -> String
{
	match var("PWD")
	{
		Ok(pwd) =>
		{ pwd }
		Err(_error) =>
		{
			print_error("could not get PWD environment variable.");
			exit(1);
		}
	}
}

