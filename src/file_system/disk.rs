use std::path::PathBuf;
use sysinfo::{
	System,
	SystemExt,
	DiskExt,
	Disk
};

pub struct MainDisk;

impl MainDisk {
	fn get_main_disk_name(partitions: &[Disk]) -> Option<String> {
		for partition in partitions {
			if partition.mount_point() == PathBuf::from("/") {
				match partition.name().to_str() {
					Some(name) => {
						let mut characters: Vec<char> = name.chars().collect();
						characters.pop();
						let name: String = characters.iter().collect();
						return Some(name);
					}
					None => {
						continue;
					}
				};
			}
		}
		None
	}

	pub fn get_usage_percentage() -> u8 {
		let mut system: System = System::new();
		system.refresh_disks_list();
		let main_disk_name: String = match MainDisk::get_main_disk_name(system.disks()) {
			Some(name) => {
				name
			}
			None => {
				return 0;
			}
		};
		let mut total_size_in_bytes: u64 = 0;
		let mut available_size_in_bytes: u64 = 0;
		for partition in system.disks() {
			if let Some(name) = partition.name().to_str() {
				let name: String = String::from(name);
				if !name.contains(&main_disk_name) {
					continue;
				}
				total_size_in_bytes += partition.total_space();
				available_size_in_bytes += partition.available_space();
			}
		}
		if total_size_in_bytes == 0 {
			return 0;
		}
		let used_size_in_bytes: u64 = total_size_in_bytes - available_size_in_bytes;
		let usage_percentage: u8 = ((used_size_in_bytes as f32 / total_size_in_bytes as f32) * 100.0) as u8;
		usage_percentage
	}
}

