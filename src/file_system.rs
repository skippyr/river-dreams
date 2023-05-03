use std::path::Path;
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
			let does_disk_contain_operating_system: bool = disk.mount_point() == Path::new("/");
			if !does_disk_contain_operating_system
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

