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
	file_system::
	{
		paths::
		{
			PathAbbreviation,
			Paths
		},
		permissions::UnixPermissions
	},
	users::Users
};
use users::get_current_uid;


pub struct DirectoryPermissions;

impl DirectoryPermissions
{
	pub fn does_user_owns_pwd() -> bool
	{
		return
			Users::is_user_root() ||
			Users::get_pwd_owner_uid() == get_current_uid()
	}
}

pub struct DirectoryEntryTypes
{
	quantity_of_executable_files: u32,
	quantity_of_symlinks: u32,
	quantity_of_broken_files: u32,
	quantity_of_hidden_files: u32
}

impl DirectoryEntryTypes
{
	pub fn from_pwd() -> DirectoryEntryTypes
	{
		let mut entry_types: DirectoryEntryTypes = DirectoryEntryTypes
		{
			quantity_of_executable_files: 0,
			quantity_of_symlinks: 0,
			quantity_of_broken_files: 0,
			quantity_of_hidden_files: 0
		};
		let stream: ReadDir = match read_dir(Paths::get_pwd())
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
				{ continue; }
			};
			let path: PathBuf = entry.path();
			let file_name = match path.file_name_as_string()
			{
				Some(file_name) =>
				{ file_name }
				None =>
				{ continue; }
			};
			let mut permissions: UnixPermissions = UnixPermissions::from(0);
			let mut is_file: bool = false;
			match path.metadata()
			{
				Ok(metadata) =>
				{
					permissions = UnixPermissions::from(metadata.permissions().mode());
					is_file = metadata.is_file();
				}
				Err(_error) =>
				{ entry_types.quantity_of_broken_files += 1; }
			}
			let symlink_metadata: Metadata = match symlink_metadata(path)
			{
				Ok(symlink_metadata) =>
				{ symlink_metadata }
				Err(_error) =>
				{ continue; }
			};
			if
				permissions.does_owner_can_execute() &&
				is_file
			{ entry_types.quantity_of_executable_files += 1; }
			if file_name.chars().collect::<Vec<char>>()[0] == '.'
			{ entry_types.quantity_of_hidden_files += 1; }
			if symlink_metadata.is_symlink()
			{ entry_types.quantity_of_symlinks += 1; }
		}
		entry_types
	}

	pub fn get_quantity_of_executable_files(&self) -> u32
	{ self.quantity_of_executable_files }

	pub fn get_quantity_of_hidden_files(&self) -> u32
	{ self.quantity_of_hidden_files }

	pub fn get_quantity_of_symlinks(&self) -> u32
	{ self.quantity_of_symlinks }

	pub fn get_quantity_of_broken_files(&self) -> u32
	{ self.quantity_of_broken_files }
}

