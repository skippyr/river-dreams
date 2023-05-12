use std::path::PathBuf;
use crate::environment::EnvironmentVariables;

pub trait PathAbbreviation
{ fn file_name_as_string(&self) -> Option<String>; }

impl PathAbbreviation for PathBuf
{
	fn file_name_as_string(&self) -> Option<String>
	{
		let file_name_as_osstr = match self.file_name()
		{
			Some(file_name_as_osstr) =>
			{ file_name_as_osstr }
			None =>
			{ return None; }
		};
		let file_name: String = match file_name_as_osstr.to_str()
		{
			Some(file_name_as_str) =>
			{ String::from(file_name_as_str) }
			None =>
			{ return None; }
		};
		Some(file_name)
	}
}

pub struct Paths;

impl Paths
{
	pub fn get_virtual_env() -> Option<String>
	{
		let virtual_env: PathBuf = match EnvironmentVariables::get_virtual_env()
		{
			Some(virtual_env) =>
			{ PathBuf::from(virtual_env) }
			None =>
			{ return None; }
		};
		virtual_env.file_name_as_string()
	}

	pub fn get_pwd() -> PathBuf
	{
		match PathBuf::from(".").canonicalize()
		{
			Ok(pwd) =>
			{ pwd }
			Err(_error) =>
			{ PathBuf::from("/") }
		}
	}
}

