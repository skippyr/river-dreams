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
	fn as_string(&self) -> String;
	fn get_base_name(&self) -> String;
	fn get_intermediate_paths(
		&self,
		repository: &Option<Repository>
	) -> Vec<String>;
	fn abbreviate(
		&self,
		repository: &Option<Repository>
	) -> String;
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

struct PathAbbreviated
{
	initial: String,
	intermediate_paths: Vec<String>,
	base_name: String
}

impl PathAbbreviated
{
	fn from(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> Self
	{
		let mut initial: String =
			if let Some(repository) = repository
			{ String::from("@") }
			else if path.as_string().contains(&EnvironmentVariables::get_home())
			{ String::from("~") }
			else
			{ String::new() };
		let intermediate_paths: Vec<String> = path.get_intermediate_paths(repository);
		let base_name: String = path.get_base_name();
		Self
		{
			initial,
			intermediate_paths,
			base_name
		}
	}

	fn as_string(&self) -> String
	{
		format!(
			"{}/{}/{}",
			self.initial,
			self.intermediate_paths.join("/"),
			self.base_name
		)
	}
}

impl PathAbbreviations for PathBuf
{
	fn as_string(&self) -> String
	{
		format!(
			"{}",
			self.display()
		)
	}

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
	
	fn get_intermediate_paths(
		&self,
		repository: &Option<Repository>
	) -> Vec<String>
	{
		let mut intermediate_paths: Vec<String> = Vec::new();
		let path: String =
			if let Some(repository) = repository
			{
				self.as_string().replacen(
					&repository.get_path().as_string(),
					"",
					1
				)
			}
			else
			{ self.as_string() };
		for split in path.split("/").collect::<Vec<&str>>()
		{
			if split != ""
			{ intermediate_paths.push(String::from(split)); }
		}
		intermediate_paths.pop();
		intermediate_paths
	}

	fn abbreviate(
		&self,
		repository: &Option<Repository>
	) -> String
	{
		let abbreviation: PathAbbreviated = PathAbbreviated::from(
			&self,
			repository
		);
		abbreviation.as_string()
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

