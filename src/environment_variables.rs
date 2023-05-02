use std::
{
	env::var,
	process::exit,
	path::PathBuf
};
use super::error_treatment::print_error_message;

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

fn get_home_directory_path() -> String
{
	match var("HOME")
	{
		Ok(home_directory_path) =>
		{ String::from(home_directory_path) }
		Err(_error) =>
		{
			print_error_message("Could not get home directory path.");
			exit(1);
		}
	}
}

pub fn get_current_directory_path() -> String
{
	match var("PWD")
	{
		Ok(current_directory_path) =>
		{
			current_directory_path.replacen(
				&get_home_directory_path(),
				"~",
				1
			)
		}
		Err(_error) =>
		{
			print_error_message("Could not get current directory path.");
			exit(1);
		}
	}
}

pub fn get_python_environment() -> Option<String>
{
	match var("VIRTUAL_ENV")
	{
		Ok(python_environment) =>
		{
			let python_environment: PathBuf = PathBuf::from(python_environment);
			match python_environment.file_name()
			{
				Some(python_environment) =>
				{
					match python_environment.to_str()
					{
						Some(python_environment) =>
						{ return Some(String::from(python_environment)) }
						None =>
						{ return None }
					};
				}
				None =>
				{ return None }
			}
		}
		Err(_error) =>
		{ None }
	}
}

