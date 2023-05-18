use crate::{
	environment::EnvironmentVariables,
	file_system::git::Repository
};
use std::
{
	path::PathBuf,
	os::unix::fs::MetadataExt
};
use users::get_current_uid;

pub trait PathAbbreviations
{
	fn get_base_name(&self) -> PathBuf;
	fn abbreviate(
		&self,
		repository: &Option<Repository>
	) -> PathBuf;
}

pub struct Paths;

impl Paths
{
	pub fn get_current_directory() -> PathBuf
	{ PathBuf::from(EnvironmentVariables::get_pwd()) }

	pub fn get_virtual_environment() -> Option<PathBuf>
	{
		match EnvironmentVariables::get_virtual_env()
		{
			Some(virtual_environment) =>
			{ Some(PathBuf::from(virtual_environment)) }
			None =>
			{ None }
		}
	}
}

struct PathsTreater;

impl PathsTreater
{
	fn shrink_aliases(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> PathBuf
	{
		let mut aliases: String = format!(
			"{}",
			path.display()
		);
		if let Some(repository) = repository
		{
			if let Some(parent) = repository.get_path().parent()
			{
				aliases = aliases.replacen(
					&format!(
						"{}",
						parent.display()
					),
					"@",
					1
				);
			}
		}
		else
		{
			aliases = aliases.replacen(
				&EnvironmentVariables::get_home(),
				"~",
				1
			);
		}
		PathBuf::from(aliases)
	}

	fn split(path: &PathBuf) -> Vec<String>
	{
		let mut splits: Vec<String> = Vec::new();
		for split in format!(
			"{}",
			path.display()
		).split("/").collect::<Vec<&str>>()
		{
			if split != ""
			{ splits.push(String::from(split)) }
		}
		splits
	}
}

impl PathAbbreviations for PathBuf
{
	fn get_base_name(&self) -> PathBuf
	{
		if let Some(name) = self.file_name()
		{
			if let Some(name) = name.to_str()
			{ return PathBuf::from(name); }
		}
		self.to_owned()
	}

	fn abbreviate(
		&self,
		repository: &Option<Repository>
	) -> PathBuf
	{
		let mut abbreviation: String = String::new();
		let aliases: PathBuf = PathsTreater::shrink_aliases(
			self,
			repository
		);
		let characters: Vec<char> = format!(
			"{}",
			aliases.display()
		).chars().collect();
		if characters[0] == '/'
		{ abbreviation.push('/'); }
		let splits: Vec<String> = PathsTreater::split(&aliases);
		for split_iterator in 0..splits.len()
		{
			if split_iterator > 0
			{ abbreviation.push('/'); }
			let split: String = splits[split_iterator].clone();
			let characters: Vec<char> = split.chars().collect();
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
			{ abbreviation.push_str(&split); }
			else if characters[0] == '.'
			{
				abbreviation.push_str(&format!(
					".{}",
					characters[1]
				));
			}
			else
			{ abbreviation.push(characters[0]); }
		}
		PathBuf::from(abbreviation)
	}
}

pub struct PathsPermissions;

impl PathsPermissions
{
	pub fn does_user_owns_current_directory() -> bool
	{
		const ROOT_UID: u32 = 0;
		let user_uid: u32 = get_current_uid();
		user_uid == ROOT_UID || match Paths::get_current_directory().metadata()
		{
			Ok(metadata) =>
			{ metadata.uid() == user_uid }
			Err(_error) =>
			{ false }
		}
	}
}

