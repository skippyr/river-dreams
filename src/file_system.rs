use super::
{
	error_treatment::print_error_message,
	environment_variables::
	{
		get_current_directory_path,
		get_current_directory_path_with_aliases
	}
};
use std::
{
	path::
	{
		Path,
		PathBuf
	},
	fs::
	{
		read_dir,
		ReadDir,
		DirEntry,
		Metadata,
		symlink_metadata
	},
	ffi::OsStr,
	process::exit,
	os::unix::{
		fs::MetadataExt,
		prelude::PermissionsExt
	}
};
use::sysinfo::
{
	System,
	SystemExt,
	Disk,
	DiskExt
};
use users::get_current_uid;

#[derive(Debug)]
pub struct QuantityOfDirectoryEntryTypes
{
	pub executable: u32,
	pub hidden: u32,
	pub symlink: u32
}

const OWNER_EXECUTION_PERMISSIONS_BIT: u32 = 0o100;

fn does_disk_contain_operating_system(disk: &Disk) -> bool
{
	return disk.mount_point() == Path::new("/")
}

pub fn get_disk_usage_percentage() -> u8
{
	let mut system: System = System::new();
	system.refresh_disks_list();
	let mut total_disk_size_in_bytes: u64 = 0;
	let mut available_disk_size_in_bytes: u64 = 0;
	for disk in system.disks()
	{
		if !does_disk_contain_operating_system(disk)
		{ continue; }
		total_disk_size_in_bytes += disk.total_space();
		available_disk_size_in_bytes += disk.available_space();
	}
	if total_disk_size_in_bytes == 0
	{
		print_error_message("Could not get total disk size.");
		exit(1);
	}
	let used_disk_size_in_bytes: u64 = total_disk_size_in_bytes - available_disk_size_in_bytes;
	((used_disk_size_in_bytes as f32 / total_disk_size_in_bytes as f32) * 100.0) as u8
}

fn get_path_splits(directory_path: String) -> Vec<String>
{
	let mut path_splits: Vec<String> = Vec::new();
	for path_split in directory_path.split("/").collect::<Vec<&str>>()
	{
		if path_split != ""
		{ path_splits.push(String::from(path_split)) }
	}
	path_splits
}

pub fn get_current_directory_path_abbreviated() -> String
{
	let mut current_directory_path_abbreviated: String = String::new();
	let current_directory_path_with_aliases: String = get_current_directory_path_with_aliases();
	if current_directory_path_with_aliases.chars().collect::<Vec<char>>()[0] == '/'
	{ current_directory_path_abbreviated.push('/'); }
	let path_splits: Vec<String> = get_path_splits(current_directory_path_with_aliases);
	for path_split_index in 0..path_splits.len()
	{
		if path_split_index == path_splits.len() - 1
		{
			current_directory_path_abbreviated.push_str(&format!(
				"{}{}",
				if path_split_index > 0
				{ "/" }
				else
				{ "" },
				path_splits[path_split_index]
			));
		}
		else
		{
			let path_split_characters: Vec<char> = path_splits[path_split_index].chars().collect();
			current_directory_path_abbreviated.push_str(&format!(
				"{}{}",
				if path_split_index > 0
				{ "/" }
				else
				{ "" },
				if path_split_characters[0] == '.'
				{
					format!(
						".{}",
						if path_split_characters.len() > 1
						{ String::from(path_split_characters[1]) }
						else
						{ String::from("") }
					)
				}
				else
				{ String::from(path_split_characters[0]) }
			));
		}
	}
	current_directory_path_abbreviated
}

pub fn does_user_have_ownership_of_current_directory_path() -> bool
{
	let current_directory_path_metadata = match Path::new(&get_current_directory_path()).metadata()
	{
		Ok(current_directory_path_metadata) =>
		{ current_directory_path_metadata }
		Err(_error) =>
		{
			print_error_message("Could not get metadata of current directory path.");
			exit(1);
		}
	};
	current_directory_path_metadata.uid() == get_current_uid()
}

fn does_the_owner_has_execution_permissions(permissions_mode: u32) -> bool
{ permissions_mode & OWNER_EXECUTION_PERMISSIONS_BIT != 0 }

pub fn get_quantity_of_directory_entry_types_in_current_directory() -> QuantityOfDirectoryEntryTypes
{
	let mut quantity_of_directory_entry_types: QuantityOfDirectoryEntryTypes = QuantityOfDirectoryEntryTypes
	{
		executable: 0,
		hidden: 0,
		symlink: 0
	};
	let directory_stream: ReadDir = match read_dir(get_current_directory_path())
	{
		Ok(directory_stream) =>
		{ directory_stream }
		Err(_error) =>
		{ return quantity_of_directory_entry_types }
	};
	for directory_entry in directory_stream
	{
		let directory_entry: DirEntry = match directory_entry
		{
			Ok(directory_entry) =>
			{ directory_entry }
			Err(_error) =>
			{ continue }
		};
		let directory_entry_path: PathBuf = directory_entry.path();
		let directory_entry_file_name_as_osstr: &OsStr = match directory_entry_path.file_name()
		{
			Some(directory_entry_file_name_as_osstr) =>
			{ directory_entry_file_name_as_osstr }
			None =>
			{ continue }
		};
		let directory_entry_file_name: String = match directory_entry_file_name_as_osstr.to_str()
		{
			Some(directory_entry_file_name_as_str) =>
			{ String::from(directory_entry_file_name_as_str) }
			None =>
			{ continue }
		};
		let directory_entry_metadata: Metadata = match directory_entry.metadata()
		{
			Ok(directory_entry_metadata) =>
			{ directory_entry_metadata }
			Err(_error) =>
			{ continue }
		};
		let directory_entry_symlink_metadata: Metadata = match symlink_metadata(directory_entry_path)
		{
			Ok(directory_entry_symlink_metadata) =>
			{ directory_entry_symlink_metadata }
			Err(_error) =>
			{ continue }
		};
		let directory_entry_permissions_mode = directory_entry_metadata.permissions().mode();
		if directory_entry_file_name.chars().collect::<Vec<char>>()[0] == '.'
		{ quantity_of_directory_entry_types.hidden += 1 }
		if directory_entry_symlink_metadata.is_symlink()
		{ quantity_of_directory_entry_types.symlink += 1 }
		if
			does_the_owner_has_execution_permissions(directory_entry_permissions_mode) &&
			!directory_entry_metadata.is_dir()
		{ quantity_of_directory_entry_types.executable += 1 }
	}
	quantity_of_directory_entry_types
}

