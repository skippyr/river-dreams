use crate::{
	environment::EnvironmentVariables,
	file_system::git::Repository
};
use std::{
	path::PathBuf,
	os::unix::fs::{
		MetadataExt,
		PermissionsExt
	},
	fs::{
		ReadDir,
		read_dir,
		DirEntry,
		Metadata,
		symlink_metadata
	}
};
use users::get_current_uid;

pub struct Paths;

impl Paths {
	pub fn get_current_directory() -> PathBuf {
		PathBuf::from(EnvironmentVariables::get_pwd())
	}

	pub fn get_virtual_environment() -> Option<PathBuf> {
		match EnvironmentVariables::get_virtual_env() {
			Some(virtual_environment) => {
				Some(PathBuf::from(virtual_environment))
			}
			None => {
				None
			}
		}
	}
}

struct PathAbbreviated {
	initial: String,
	intermediate_paths: Vec<String>,
	base_name: String
}

impl PathAbbreviated {
	fn as_string(&self) -> String {
		format!(
			"{}{}{}{}{}",
			self.initial,
			if
				self.initial != "/" &&
				self.base_name != "" {
				String::from("/")
			} else {
				String::new()
			},
			self.intermediate_paths.join("/"),
			if self.intermediate_paths.len() > 0 {
				String::from("/")
			} else {
				String::new()
			},
			self.base_name
		)
	}
}

pub struct PathTreater;

impl PathTreater {
	pub fn to_string(path: &PathBuf) -> String {
		format!("{}", path.display())
	}

	pub fn get_base_name(path: &PathBuf) -> String {
		if let Some(base_name) = path.file_name() {
			if let Some(base_name) = base_name.to_str() {
				return String::from(base_name);
			}
		}
		Self::to_string(path)
	}

	fn abbreviate_initial(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> String {
		if let Some(repository) = repository {
			String::from(format!(
				"@/{}",
				PathTreater::get_base_name(&repository.get_path())
			))
		} else if Self::to_string(path).contains(&EnvironmentVariables::get_home()) {
			String::from("~")
		} else {
			String::from("/")
		}
	}

	fn abbreviate_base_name(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> String {
		let base_name: String = Self::get_base_name(&PathBuf::from(
			if let Some(repository) = repository {
				Self::to_string(path).replacen(
					&PathTreater::to_string(&repository.get_path()),
					"",
					1
				)
			} else {
				Self::to_string(path).replacen(
					&EnvironmentVariables::get_home(),
					"",
					1
				)
			}
		));
		if base_name == "/" {
			String::new()
		} else {
			base_name
		}
	}

	fn abbreviate_intermediate_paths(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> Vec<String> {
		let mut intermediate_paths: Vec<String> = Vec::new();
		let short_path: PathBuf = if let Some(repository) = repository {
			PathBuf::from(Self::to_string(path).replacen(
				&Self::to_string(&repository.get_path()),
				"",
				1
			))
		} else if Self::to_string(path).contains(&EnvironmentVariables::get_home()) {
			PathBuf::from(Self::to_string(path).replacen(
				&EnvironmentVariables::get_home(),
				"",
				1
			))
		} else {
			path.clone()
		};
		for split in Self::to_string(&short_path).split("/").collect::<Vec<&str>>() {
			if split != "" {
				intermediate_paths.push(
					if intermediate_paths.len() > 0 {
						let last_index: usize = intermediate_paths.len() - 1;
						let last_path: String = intermediate_paths[last_index].clone();
						format!("{}/{}", last_path, split)
					} else {
						let path: String = if let Some(repository) = repository {
							Self::to_string(&repository.get_path())
						} else if Self::to_string(path).contains(&EnvironmentVariables::get_home()) {
							EnvironmentVariables::get_home()
						} else {
							String::new() 
						};
						format!("{}/{}", path, split)
					}
				);
			}
		}
		intermediate_paths.pop();
		let intermediate_paths: Vec<String> = intermediate_paths
			.iter()
			.map(
				|intermediate_path|
				Self::abbreviate_base_name_using_entries(intermediate_path.clone())
			)
			.collect();
		intermediate_paths
	}

	fn abbreviate_base_name_using_entries(path: String) -> String {
		let parent: String = match PathBuf::from(&path).parent() {
			Some(parent) => {
				Self::to_string(&parent.to_path_buf())
			}
			None => {
				return path.clone();
			}
		};
		let stream: ReadDir = match read_dir(parent) {
			Ok(stream) => {
				stream
			}
			Err(_error) => {
				return path.clone();
			}
		};
		let mut quantity_of_characters_to_include: usize = 0;
		let path_base_name: String = Self::get_base_name(&PathBuf::from(&path));
		let path_characters: Vec<char> = path_base_name.chars().collect();
		for entry in stream {
			let entry: DirEntry = match entry {
				Ok(entry) => {
					entry
				}
				Err(_error) => {
					continue;
				}
			};
			let entry_name: String = PathTreater::get_base_name(&entry.path());
			if entry_name == path_base_name {
				continue;
			}
			let entry_characters: Vec<char> = Self::get_base_name(&entry.path()).chars().collect();
			for character_index in 0..path_characters.len() {
				if character_index == entry_characters.len() {
					break;
				}
				let entry_character: char = entry_characters[character_index];
				let path_character: char = path_characters[character_index];
				if 
					entry_character != path_character ||
					character_index == path_characters.len() - 1 {
					let real_index = character_index + 1;
					if real_index > quantity_of_characters_to_include {
						quantity_of_characters_to_include = real_index;
					}
					break;
				}
			}
		}
		if
			(
				path_characters[0] == '.' &&
				quantity_of_characters_to_include > 1
			) ||
			quantity_of_characters_to_include > 1 {
			let mut characters: Vec<char> = Vec::new();
			for character_iterator in 0..path_characters.len() {
				if character_iterator == quantity_of_characters_to_include {
					break;
				}
				let path_character: char = path_characters[character_iterator];
				characters.push(path_character);
			}
			return characters.iter().collect();
		}
		if path_characters[0] == '.' {
			format!(".{}", path_characters[1])
		} else {
			format!("{}", path_characters[0])
		}
	}

	pub fn abbreviate(
		path: &PathBuf,
		repository: &Option<Repository>
	) -> String {
		let initial: String = Self::abbreviate_initial(path, repository);
		let base_name: String = Self::abbreviate_base_name(path, repository);
		let intermediate_paths: Vec<String> = Self::abbreviate_intermediate_paths(
			path,
			repository
		);
		PathAbbreviated {
			initial,
			intermediate_paths,
			base_name
		}.as_string()
	}
}

pub struct PathsPermissions;

impl PathsPermissions {
	pub fn does_user_owns_current_directory() -> bool {
		const ROOT_UID: u32 = 0;
		let user_uid: u32 = get_current_uid();
		user_uid == ROOT_UID ||
		match Paths::get_current_directory().metadata() {
			Ok(metadata) => {
				metadata.uid() == user_uid
			}
			Err(_error) => {
				false
			}
		}
	}

	pub fn does_user_can_execute(mode: u32) -> bool {
		const UNIX_OWNER_EXECUTION_PERMISSIONS_BIT: u32 = 0o100;
		mode & UNIX_OWNER_EXECUTION_PERMISSIONS_BIT != 0
	}
}

pub struct PathEntryTypes {
	quantity_of_symlinks: u32,
	quantity_of_broken_files: u32,
	quantity_of_executable_files: u32,
	quantity_of_hidden_files: u32
}

impl PathEntryTypes {
	fn new() -> Self {
		Self {
			quantity_of_symlinks: 0,
			quantity_of_broken_files: 0,
			quantity_of_executable_files: 0,
			quantity_of_hidden_files: 0
		}
	}

	pub fn from_current_directory() -> Self {
		let mut entry_types: PathEntryTypes = PathEntryTypes::new();
		let stream: ReadDir = match read_dir(Paths::get_current_directory()) {
			Ok(stream) => {
				stream
			}
			Err(_error) => {
				return entry_types;
			}
		};
		for entry in stream {
			let entry: DirEntry = match entry {
				Ok(entry) => {
					entry
				}
				Err(_error) => {
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let name: String = PathTreater::to_string(&PathBuf::from(PathTreater::get_base_name(&entry.path())));
			let characters: Vec<char> = name.chars().collect();
			let metadata: Metadata = match entry.path().metadata() {
				Ok(metadata) => {
					metadata
				}
				Err(_error) => {
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let symlink_metadata: Metadata = match symlink_metadata(entry.path()) {
				Ok(metadata) => {
					metadata
				}
				Err(_error) => {
					entry_types.quantity_of_broken_files += 1;
					continue;
				}
			};
			let mode: u32 = metadata.permissions().mode();
			if
				PathsPermissions::does_user_can_execute(mode) &&
				metadata.is_file() {
				entry_types.quantity_of_executable_files += 1;
			}
			if characters[0] == '.' {
				entry_types.quantity_of_hidden_files += 1;
			}
			if symlink_metadata.is_symlink() {
				entry_types.quantity_of_symlinks += 1;
			}
		}
		entry_types
	}

	pub fn get_quantity_of_symlinks(&self) -> u32 {
		self.quantity_of_symlinks
	}

	pub fn get_quantity_of_broken_files(&self) -> u32 {
		self.quantity_of_broken_files
	}

	pub fn get_quantity_of_executable_files(&self) -> u32 {
		self.quantity_of_executable_files
	}

	pub fn get_quantity_of_hidden_files(&self) -> u32 {
		self.quantity_of_hidden_files
	}
}

