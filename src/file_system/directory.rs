use std::{
	fs::
	{
		Metadata,
		ReadDir,
		read_dir,
		DirEntry,
		symlink_metadata
	},
	path::PathBuf,
	os::unix::fs::PermissionsExt
};
use crate::
{
	file_system::paths::
	{
		PathAbbreviation,
		Paths
	},
	users::UnixUsers
};
pub struct DirectoryOwnership;

impl DirectoryOwnership
{
	pub fn does_current_user_owns_pwd() -> bool
	{ UnixUsers::get_owner_uid_of_pwd() == UnixUsers::get_current_user_uid() }
}

struct UnixPermissions
{ mode: u32 }

impl UnixPermissions
{
	pub fn from(mode: u32) -> UnixPermissions
	{ UnixPermissions { mode } }

	pub fn does_owner_can_execute(&self) -> bool
	{
		const UNIX_OWNER_EXECUTION_PERMISSIONS_BIT: u32 = 0o100;
		self.mode & UNIX_OWNER_EXECUTION_PERMISSIONS_BIT != 0
	}
}

pub struct DirectoryEntryTypes
{
	quantity_of_executable_files: u32,
	quantity_of_symlinks: u32,
	quantity_of_hidden_files: u32
}

impl DirectoryEntryTypes
{
	pub fn from_pwd() -> DirectoryEntryTypes
	{
		let mut directory_entry_types: DirectoryEntryTypes = DirectoryEntryTypes
		{
			quantity_of_executable_files: 0,
			quantity_of_symlinks: 0,
			quantity_of_hidden_files: 0
		};
		let directory_stream: ReadDir = match read_dir(Paths::get_pwd())
		{
			Ok(directory_stream) =>
			{ directory_stream }
			Err(_error) =>
			{ return directory_entry_types; }
		};
		for directory_entry in directory_stream
		{
			let directory_entry: DirEntry = match directory_entry
			{
				Ok(directory_entry) =>
				{ directory_entry }
				Err(_error) =>
				{ continue; }
			};
			let directory_entry_path: PathBuf = directory_entry.path();
			let directory_entry_file_name = match directory_entry_path.file_name_as_string()
			{
				Some(directory_entry_file_name) =>
				{ directory_entry_file_name }
				None =>
				{ continue; }
			};
			let directory_entry_symlink_metadata: Metadata = match symlink_metadata(directory_entry_path)
			{
				Ok(directory_entry_symlink_metadata) =>
				{ directory_entry_symlink_metadata }
				Err(_error) =>
				{ continue; }
			};
			let directory_entry_metadata: Metadata = match directory_entry.metadata()
			{
				Ok(directory_entry_metadata) =>
				{ directory_entry_metadata }
				Err(_error) =>
				{ continue; }
			};
			let directory_entry_permissions_mode: u32 = directory_entry_metadata.permissions().mode();
			if
				UnixPermissions::from(directory_entry_permissions_mode).does_owner_can_execute() &&
				!directory_entry_metadata.is_dir()
			{ directory_entry_types.quantity_of_executable_files += 1; }
			if directory_entry_file_name.chars().collect::<Vec<char>>()[0] == '.'
			{ directory_entry_types.quantity_of_hidden_files += 1; }
			if directory_entry_symlink_metadata.is_symlink()
			{ directory_entry_types.quantity_of_symlinks += 1; }
		}
		directory_entry_types
	}

	pub fn get_quantity_of_executable_files(&self) -> u32
	{ self.quantity_of_executable_files }

	pub fn get_quantity_of_hidden_files(&self) -> u32
	{ self.quantity_of_hidden_files }

	pub fn get_quantity_of_symlinks(&self) -> u32
	{ self.quantity_of_symlinks }
}

