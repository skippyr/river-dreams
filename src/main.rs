use std::path::PathBuf;

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
    print!("%F{{4}}  %f%m%F{{4}}@%f{local_ip}");
}

fn get_main_disk_name(partitions: &[sysinfo::Disk]) -> Option<String> {
    for partition in partitions {
        if partition.mount_point() == PathBuf::from("/") {
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

fn main() {
    print_commands_separator();
    print!("%F{{3}}:«");
    print_local_ip();
    print_spacing();
    print_disk_usage();
    println!("»:");
}
