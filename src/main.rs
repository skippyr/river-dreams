#![allow(nonstandard_style)]

use chrono::{Datelike, Timelike};
use std::path::PathBuf;
use sysinfo::{DiskExt, SystemExt};

fn Throw_Error(description: impl std::fmt::Display) -> !
{
	eprintln!("Error Report - river-dreams (1):");
	eprintln!("   Description:");
	eprintln!("      {description}");
	std::process::exit(1);
}

fn Print_Spacing()
{
	print!("  ");
}

fn Print_Commands_Separator()
{
	let size = crossterm::terminal::size().unwrap_or_else(|_| {
		Throw_Error("can not get terminal size.");
	});
	for column in 0..size.0 {
		if column % 2 == 0 {
			print!("%F{{3}}⊵%f");
		}
		else {
			print!("%F{{1}}⊼%f");
		}
	}
}

fn Print_Local_Ip_Address()
{
	let local_ip = match local_ip_address::local_ip() {
		Ok(local_ip) => local_ip.to_string(),
		Err(_) => "No Address Found".to_string()
	};
	print!("%F{{4}} %f%m%F{{4}}@%f{local_ip}");
}

fn Get_Main_Disk_Name(partitions: &[sysinfo::Disk]) -> Option<String>
{
	for partition in partitions {
		if partition.mount_point() == std::path::PathBuf::from("/") {
			if let Some(name) = partition.name().to_str() {
				let mut characters: Vec<char> = name.chars().collect();
				characters.pop();
				let name: String = characters.iter().collect();
				return Some(name);
			}
		}
	}
	None
}

fn Print_Disk_Usage()
{
	let mut system = sysinfo::System::new();
	system.refresh_disks_list();
	let main_disk_name = Get_Main_Disk_Name(system.disks()).unwrap_or_else(|| {
		Throw_Error("can not get main disk name.");
	});
	let mut total_size_in_bytes: u64 = 0;
	let mut available_size_in_bytes: u64 = 0;
	for partition in system.disks() {
		if let Some(name) = partition.name().to_str() {
			if name.contains(&main_disk_name) {
				total_size_in_bytes += partition.total_space();
				available_size_in_bytes += partition.available_space();
			}
		}
	}
	let mut usage_percentage: u8 = 0;
	if total_size_in_bytes != 0 {
		let used_size_in_bytes: u64 = total_size_in_bytes - available_size_in_bytes;
		usage_percentage = ((used_size_in_bytes as f32 / total_size_in_bytes as f32) * 100.0) as u8;
	}
	print!("%F{{3}}󰋊 %f{usage_percentage}%%");
}

fn Get_Month(moment: &chrono::DateTime<chrono::Local>) -> String
{
	match moment.month0() {
		1 => "Feb".to_string(),
		2 => "Mar".to_string(),
		3 => "Apr".to_string(),
		4 => "May".to_string(),
		5 => "Jun".to_string(),
		6 => "Jul".to_string(),
		7 => "Aug".to_string(),
		8 => "Sep".to_string(),
		9 => "Oct".to_string(),
		10 => "Nov".to_string(),
		_ => "Jan".to_string()
	}
}

fn Get_Week_Day(moment: &chrono::DateTime<chrono::Local>) -> String
{
	match moment.weekday() {
		chrono::Weekday::Sun => "Sun".to_string(),
		chrono::Weekday::Mon => "Mon".to_string(),
		chrono::Weekday::Tue => "Tue".to_string(),
		chrono::Weekday::Wed => "Wed".to_string(),
		chrono::Weekday::Thu => "Thu".to_string(),
		chrono::Weekday::Fri => "Fri".to_string(),
		chrono::Weekday::Sat => "Sat".to_string()
	}
}

fn Get_Ordinal(number: u32) -> String
{
	if number - 1 % 10 == 0 {
		"st".to_string()
	}
	else if number - 2 % 10 == 0 {
		"nd".to_string()
	}
	else if number - 3 % 10 == 0 {
		"rd".to_string()
	}
	else {
		"th".to_string()
	}
}

fn Print_Calendar(moment: &chrono::DateTime<chrono::Local>) {
	let month = Get_Month(&moment);
	let day = moment.day();
	let ordinal = Get_Ordinal(day);
	let week_day = Get_Week_Day(&moment);
	print!("%F{{1}}󰸗 %f({week_day}) {month} {day}{ordinal}")
}

fn Format_Time(time: u32) -> String
{
	format!("{}{time}", if time < 10 {"0"} else {""})
}

fn Print_Clock(moment: &chrono::DateTime<chrono::Local>)
{
	let hour = moment.hour();
	let minute = moment.minute();
	if hour < 6 {
		print!("%F{{6}}󰭎 %f");
	}
	else if hour < 12 {
		print!("%F{{1}}󰖨  %f");
	}
	else if hour < 18 {
		print!("%F{{4}} %f");
	}
	else {
		print!("%F{{3}}󰽥 %f");
	}
	print!("{}h{}m", Format_Time(hour), Format_Time(minute));
}

fn Print_User()
{
	print!("%(#.{{%F{{1}}#%F{{3}}}}.)")
}

fn Print_Status()
{
	print!("{{%(?.≗.%F{{1}}⨲%F{{3}})}}");
}

fn Print_Virtual_Environment()
{
	if let Ok(virtual_environment) = std::env::var("VIRTUAL_ENV") {
		let error_description = "can not get virtual environment";
		let virtual_environment = PathBuf::from(virtual_environment);
		let virtual_environment = virtual_environment
			.file_name()
			.unwrap_or_else(|| {
				Throw_Error(error_description);
			})
			.to_str()
			.unwrap_or_else(|| {
				Throw_Error(error_description);
			});
		print!("%F{{6}}(%f{virtual_environment}%F{{6}}) ");
	}
}

fn main()
{
	let moment = chrono::Local::now();
	Print_Commands_Separator();
	print!("%F{{3}}:«");
	Print_Local_Ip_Address();
	Print_Spacing();
	Print_Disk_Usage();
	Print_Spacing();
	Print_Calendar(&moment);
	Print_Spacing();
	Print_Clock(&moment);
	println!("%F{{3}}»:");
	Print_User();
	Print_Status();
	print!("⤐  ");
	Print_Virtual_Environment();
	print!("%F{{1}}%1~ %F{{6}}⩺ %f");
}
