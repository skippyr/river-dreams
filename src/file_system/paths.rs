use std::path::PathBuf;
use crate::environment::EnvironmentVariables;

pub trait PathAbbreviation
{
	fn file_name_as_string(&self) -> Option<String>;
	fn as_string(&self) -> String;
	fn as_string_with_home_alias(&self) -> String;
	fn split_by_slash_using_home_alias(&self) -> Vec<String>;
	fn as_abbreviated_string(&self) -> String;
}

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

	fn as_string(&self) -> String
	{
		match self.to_str()
		{
			Some(path_as_str) =>
			{ String::from(path_as_str) }
			None =>
			{ String::new() }
		}
	}

	fn as_string_with_home_alias(&self) -> String
	{
		self
			.as_string()
			.replacen(
			&EnvironmentVariables::get_home(),
			"~",
			1
			)
	}

	fn split_by_slash_using_home_alias(&self) -> Vec<String>
	{
		let path_with_home_alias: String = self.as_string_with_home_alias();
		let raw_splits: Vec<&str> = path_with_home_alias.split("/").collect::<Vec<&str>>();
		let mut splits: Vec<String> = Vec::new();
		for raw_split in raw_splits
		{
			if raw_split != ""
			{ splits.push(String::from(raw_split)); }
		}
		splits
	}

	fn as_abbreviated_string(&self) -> String
	{
		let mut path_abbreviated: String = String::new();
		let splits: Vec<String> = self.split_by_slash_using_home_alias();
		if self.as_string_with_home_alias().chars().collect::<Vec<char>>()[0] == '/'
		{ path_abbreviated.push('/'); }
		for split_iterator in 0..splits.len()
		{
			let split: String = splits[split_iterator].clone();
			let split_characters: Vec<char> = split.chars().collect();
			path_abbreviated.push_str(&format!(
				"{}",
				if split_iterator > 0
				{ String::from("/") }
				else
				{ String::new() }
			));
			if split_iterator == splits.len() - 1
			{
				path_abbreviated.push_str(&format!(
					"{}",
					split
				));
			}
			else if split_characters[0] == '.'
			{
				path_abbreviated.push_str(&format!(
					".{}",
					split_characters[1]
				));
			}
			else
			{
				path_abbreviated.push_str(&format!(
					"{}",
					split_characters[0]
				));
			}
		}
		path_abbreviated
	}
}

pub struct Paths;

impl Paths
{
	pub fn get_pwd_abbreviated() -> String
	{ PathBuf::from(Paths::get_pwd()).as_abbreviated_string() }

	pub fn get_virtual_env_abbreviated() -> Option<String>
	{
		match EnvironmentVariables::get_virtual_env()
		{
			Some(virtual_env) =>
			{ Some(PathBuf::from(virtual_env).as_abbreviated_string()) }
			None =>
			{ None }
		}
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

