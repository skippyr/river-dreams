use std::
{
	env::var,
	process::exit,
	path::Path,
	ffi::OsStr
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
		{ home_directory_path }
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
		{ current_directory_path }
		Err(_error) =>
		{
			print_error_message("Could not get current directory path.");
			exit(1);
		}
	}
}

pub fn get_current_directory_path_with_aliases() -> String
{
	get_current_directory_path().replacen(
		&get_home_directory_path(),
		"~",
		1
	)
}

pub fn get_python_environment() -> Option<String>
{
	let python_environment_path_as_string: String = match var("VIRTUAL_ENV")
	{
		Ok(python_environment_path) =>
		{ python_environment_path }
		Err(_error) =>
		{ return None }
	};
	let python_environment_path: &Path = Path::new(&python_environment_path_as_string);
	let python_environment_as_osstr: &OsStr = match python_environment_path.file_name()
	{
		Some(python_environment_as_osstr) =>
		{ python_environment_as_osstr }
		None =>
		{ return None }
	};
	let python_environment: String = match python_environment_as_osstr.to_str()
	{
		Some(python_environment) =>
		{ String::from(python_environment) }
		None =>
		{ return None }
	};
	Some(python_environment)
}

