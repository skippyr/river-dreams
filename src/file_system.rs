use super::environment_variables::
{
	get_pwd,
	get_home,
	get_virtual_env
};
use std::path::
{
	Path,
	PathBuf
};
use::sysinfo::
{
	System,
	SystemExt,
	DiskExt
};

pub struct MainDisk
{
	total_size_in_bytes: u64,
	in_use_size_in_bytes: u64
}

impl MainDisk
{
	pub fn new() -> MainDisk
	{
		let mut system: System = System::new();
		system.refresh_disks_list();
		let mut total_size_in_bytes: u64 = 0;
		let mut available_size_in_bytes: u64 = 0;
		for disk in system.disks()
		{
			let is_main_disk: bool = disk.mount_point() == Path::new("/");
			if !is_main_disk
			{ continue; }
			total_size_in_bytes += disk.total_space();
			available_size_in_bytes += disk.available_space();
		}
		let in_use_size_in_bytes: u64 = total_size_in_bytes - available_size_in_bytes;
		MainDisk
		{
			total_size_in_bytes,
			in_use_size_in_bytes
		}
	}

	fn get_in_use_percentage(&self) -> u8
	{
		if self.total_size_in_bytes == 0
		{ 0 }
		else
		{
			((self.in_use_size_in_bytes as f32 / self.total_size_in_bytes as f32) * 100.0) as u8
		}
	}

	pub fn get_in_use_percentage_as_string(&self) -> String
	{
		format!(
			"{}%%",
			self.get_in_use_percentage()
		)
	}
}

pub fn get_pwd_as_path_buff() -> PathBuf
{ PathBuf::from(get_pwd()) }

pub fn get_virtual_env_as_path_buff() -> Option<PathBuf>
{
	match get_virtual_env()
	{
		Some(virtual_env) =>
		{ Some(PathBuf::from(virtual_env)) }
		None =>
		{ None }
	}
}

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
			&get_home(),
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
			if split_characters[0] == '.'
			{
				path_abbreviated.push_str(&format!(
					".{}",
					split_characters[1]
				));
			}
			else if split_iterator != splits.len() - 1
			{
				path_abbreviated.push_str(&format!(
					"{}",
					split_characters[0]
				));
			}
			else
			{
				path_abbreviated.push_str(&format!(
					"{}",
					split
				));
			}
		}
		path_abbreviated
	}
}

