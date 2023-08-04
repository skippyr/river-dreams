use std::path::PathBuf;

use chrono::{Datelike, Timelike};
use sysinfo::{DiskExt, SystemExt};

fn throw_error(description: impl std::fmt::Display) -> ! {
    eprintln!("Error Report - river-dreams (1):");
    eprintln!("   Description:");
    eprintln!("      {description}");
    std::process::exit(1);
}

fn print_spacing() {
    print!("  ");
}

fn print_commands_separator() {
    let size = crossterm::terminal::size().unwrap_or_else(|_| {
        throw_error("can not get terminal size.");
    });
    for column in 0..size.0 {
        if column % 2 == 0 {
            print!("%F{{3}}⊵%f");
        } else {
            print!("%F{{1}}⊼%f");
        }
    }
}

fn print_local_ip() {
    let local_ip = match local_ip_address::local_ip() {
        Ok(local_ip) => local_ip.to_string(),
        Err(_) => "No Address Found".to_string(),
    };
    print!("%F{{4}} %f%m%F{{4}}@%f{local_ip}");
}

fn get_main_disk_name(partitions: &[sysinfo::Disk]) -> Option<String> {
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

fn print_disk_usage() {
    let mut system = sysinfo::System::new();
    system.refresh_disks_list();
    let main_disk_name = get_main_disk_name(system.disks()).unwrap_or_else(|| {
        throw_error("can not get main disk name.");
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

fn get_month(moment: &chrono::DateTime<chrono::Local>) -> String {
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
        _ => "Jan".to_string(),
    }
}

fn get_week_day(moment: &chrono::DateTime<chrono::Local>) -> String {
    match moment.weekday() {
        chrono::Weekday::Sun => "Sun".to_string(),
        chrono::Weekday::Mon => "Mon".to_string(),
        chrono::Weekday::Tue => "Tue".to_string(),
        chrono::Weekday::Wed => "Wed".to_string(),
        chrono::Weekday::Thu => "Thu".to_string(),
        chrono::Weekday::Fri => "Fri".to_string(),
        chrono::Weekday::Sat => "Sat".to_string(),
    }
}

fn get_ordinal(number: u32) -> String {
    if number - 1 % 10 == 0 {
        "st".to_string()
    } else if number - 2 % 10 == 0 {
        "nd".to_string()
    } else if number - 3 % 10 == 0 {
        "rd".to_string()
    } else {
        "th".to_string()
    }
}

fn print_calendar(moment: &chrono::DateTime<chrono::Local>) {
    let month = get_month(&moment);
    let day = moment.day();
    let ordinal = get_ordinal(day);
    let week_day = get_week_day(&moment);
    print!("%F{{1}}󰸗 %f({week_day}) {month} {day}{ordinal}")
}

fn format_time(time: u32) -> String {
    format!("{}{time}", if time < 10 { "0" } else { "" })
}

fn print_clock(moment: &chrono::DateTime<chrono::Local>) {
    let hour = moment.hour();
    let minute = moment.minute();
    if hour < 6 {
        print!("%F{{6}}󰭎 %f");
    } else if hour < 12 {
        print!("%F{{1}}󰖨  %f");
    } else if hour < 18 {
        print!("%F{{4}} %f");
    } else {
        print!("%F{{3}}󰽥 %f");
    }
    print!("{}h{}m", format_time(hour), format_time(minute));
}

fn print_status() {
    print!("{{%(?.≗.%F{{1}}⨲%F{{3}})}}");
}

fn print_virtual_environment() {
    if let Ok(virtual_environment) = std::env::var("VIRTUAL_ENV") {
        let error_description = "can not get virtual environment";
        let virtual_environment = PathBuf::from(virtual_environment);
        let virtual_environment = virtual_environment
            .file_name()
            .unwrap_or_else(|| {
                throw_error(error_description);
            })
            .to_str()
            .unwrap_or_else(|| {
                throw_error(error_description);
            });
        print!("%F{{6}}(%f{virtual_environment}%F{{6}})");
    }
}

fn main() {
    let moment: chrono::DateTime<chrono::Local> = chrono::Local::now();
    print_commands_separator();
    print!("%F{{3}}:«");
    print_local_ip();
    print_spacing();
    print_disk_usage();
    print_spacing();
    print_calendar(&moment);
    print_spacing();
    print_clock(&moment);
    println!("%F{{3}}»:");
    print_status();
    print!("⤐  %F{{1}}%1~ %F{{6}}⩺ ");
    print!("%f");
    print_virtual_environment();
}
