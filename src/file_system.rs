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
	path::Path,
	process::exit,
	os::unix::fs::MetadataExt
};
use::sysinfo::
{
	System,
	SystemExt,
	Disk,
	DiskExt
};
use users::get_current_uid;

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

