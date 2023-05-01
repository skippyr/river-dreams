use super::error_treatment::print_error_message;
use std::process::exit;
use::sysinfo::
{
	System,
	SystemExt,
	DiskExt
};

pub fn get_disk_usage_percentage() -> u8
{
	let mut system: System = System::new();
	system.refresh_disks_list();
	let mut total_disk_size_in_bytes: u64 = 0;
	let mut available_disk_size_in_bytes: u64 = 0;
	for disk in system.disks()
	{
		total_disk_size_in_bytes += disk.total_space();
		available_disk_size_in_bytes += disk.available_space();
	}
	if total_disk_size_in_bytes == 0
	{
		print_error_message("Could not get disk size.");
		exit(1);
	}
	let used_disk_size_in_bytes: u64 = total_disk_size_in_bytes - available_disk_size_in_bytes;
	((used_disk_size_in_bytes as f32 / total_disk_size_in_bytes as f32) * 100.0) as u8
}

