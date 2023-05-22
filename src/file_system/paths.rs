use crate::{
	environment::EnvironmentVariables,
	file_system::git::Repository
};
use std::
{
	path::PathBuf,
	os::unix::fs::
	{
		MetadataExt,
		PermissionsExt
	},
	fs::
	{
		ReadDir,
		read_dir,
		DirEntry,
		Metadata,
		symlink_metadata
	}
};
use users::get_current_uid;

pub trait PathAbbreviations
{
	fn get_base_name(&self) -> String;
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
	fn get_base_name(&self) -> String
	{
		if let Some(name) = self.file_name()
		{
			if let Some(name) = name.to_str()
			{ return String::from(name); }
		}
		format!(
			"{}",
			self.display()
		)
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

	pub fn does_user_can_execute(mode: u32) -> bool
	{
		const UNIX_OWNER_EXECUTION_PERMISSIONS_BIT: u32 = 0o100;
		mode & UNIX_OWNER_EXECUTION_PERMISSIONS_BIT != 0
	}
}

pub struct PathEntryTypes
{
	quantity_of_symlinks: u32,
	quantity_of_broken_files: u32,
	quantity_of_executable_files: u32,
	quantity_of_hidden_files: u32
}

impl PathEntryTypes
{
	fn new() -> Self
	{
		Self
		{
			quantity_of_symlinks: 0,
			quantity_of_broken_files: 0,
			quantity_of_executable_files: 0,
			quantity_of_hidden_files: 0
		}
	}

	pub fn from_current_directory() -> Self
	{
		let mut entry_types: PathEntryTypes = PathEntryTypes::new();
		let stream: ReadDir = match read_dir(Paths::get_current_directory())
		{
			Ok(stream) =>
			{ stream }
			Err(_error) =>
			{ return entry_types; }
		};
		for entry in stream
		{
			let entry: DirEntry = match entry
			{
				Ok(entry) =>
				{ entry }
				Err(_error) =>
				{
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let name: String = entry.path().get_base_name();
			let characters: Vec<char> = name.chars().collect();
			let metadata: Metadata = match entry.path().metadata()
			{
				Ok(metadata) =>
				{ metadata }
				Err(_error) =>
				{
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let symlink_metadata: Metadata = match symlink_metadata(entry.path())
			{
				Ok(metadata) =>
				{ metadata }
				Err(_error) =>
				{
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let mode: u32 = metadata.permissions().mode();
			if
				PathsPermissions::does_user_can_execute(mode) &&
				metadata.is_file()
			{ entry_types.quantity_of_executable_files += 1; }
			if characters[0] == '.'
			{ entry_types.quantity_of_hidden_files += 1; }
			if symlink_metadata.is_symlink()
			{ entry_types.quantity_of_symlinks += 1; }
		}
		entry_types
	}

	pub fn get_quantity_of_symlinks(&self) -> u32
	{ self.quantity_of_symlinks }

	pub fn get_quantity_of_broken_files(&self) -> u32
	{ self.quantity_of_broken_files }

	pub fn get_quantity_of_executable_files(&self) -> u32
	{ self.quantity_of_executable_files }

	pub fn get_quantity_of_hidden_files(&self) -> u32
	{ self.quantity_of_hidden_files }
}

