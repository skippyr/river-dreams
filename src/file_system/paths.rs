use crate::{
	environment::EnvironmentVariables,
	file_system::git::Repository
};
use std::path::PathBuf;

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
			aliases = aliases.replace(
				&format!(
					"{}",
					repository.get_path().display()
				),
				"@"
			);
		}
		else
		{
			aliases = aliases.replace(
				&EnvironmentVariables::get_home(),
				"~"
			)
		}
		PathBuf::from(aliases)
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
		let aliases: PathBuf = Paths::shrink_aliases(
			self,
			repository
		);
		aliases
	}
}

