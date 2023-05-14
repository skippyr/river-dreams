use std::path::PathBuf;
use crate::
{
	environment::EnvironmentVariables,
	git::GitRepository
};

pub trait PathAbbreviation
{
	fn file_name_as_string(&self) -> Option<String>;
	fn as_string(&self) -> String;
	fn get_parent_as_string(&self) -> String;
	fn get_path_with_aliases(
		&self,
		repository: &Option<GitRepository>
	) -> String;
	fn split_by_slash(
		&self,
		path_with_aliases: String
	) -> Vec<String>;
	fn as_abbreviated_string(
		&self,
		repository: &Option<GitRepository>
	) -> String;
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
		format!(
			"{}",
			self.display()
		)
	}

	fn get_parent_as_string(&self) -> String
	{
		match self.parent()
		{
			Some(parent) =>
			{
				format!(
					"{}",
					parent.display()
				)
			}
			None =>
			{ self.as_string() }
		}
	}

	fn get_path_with_aliases(
		&self,
		repository: &Option<GitRepository>
	) -> String
	{
		match repository
		{
			Some(repository) =>
			{
				self
					.as_string()
					.replacen(
						&repository.get_path().get_parent_as_string(),
						"@",
						1
					)
			}
			None =>
			{
				self
					.as_string()
					.replacen(
						&EnvironmentVariables::get_home(),
						"~",
						1
					)
			}
		}
	}

	fn split_by_slash(
		&self,
		path: String
	) -> Vec<String>
	{
		let raw_splits: Vec<&str> = path.split("/").collect::<Vec<&str>>();
		let mut splits: Vec<String> = Vec::new();
		for raw_split in raw_splits
		{
			if raw_split != ""
			{ splits.push(String::from(raw_split)); }
		}
		splits
	}

	fn as_abbreviated_string(
		&self,
		repository: &Option<GitRepository>
	) -> String
	{
		let mut path_abbreviated: String = String::new();
		let path_with_aliases: String = self.get_path_with_aliases(repository);
		let splits: Vec<String> = self.split_by_slash(path_with_aliases.clone());
		if path_with_aliases.chars().collect::<Vec<char>>()[0] == '/'
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
			let is_git_repository: bool = match repository
			{
				Some(_repository) =>
				{ true }
				None =>
				{ false }
			};
			if
				split_iterator == splits.len() - 1 ||
				(
					split_iterator == 1 &&
					is_git_repository
				)
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

