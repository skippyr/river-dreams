#![allow(nonstandard_style)]

use chrono::{Datelike, Timelike};
use sysinfo::{DiskExt, SystemExt};

fn Throw_Error(description: impl std::fmt::Display) -> !
{
    eprintln!("river-dreams: {description}");
    std::process::exit(1);
}

fn Print_Spacing()
{
    print!("  ");
}

fn Print_Commands_Separator()
{
    let size = crossterm::terminal::size().unwrap_or_else(
        |__error| {Throw_Error(
            "can not get terminal size.");});
    for column in 0..size.0 {
        print!("{}%f", if column % 2 == 0 {"%F{3}⊵"} else {"%F{1}⊼"});
    }
}

fn Print_Local_Ip_Address()
{
    let ip = match local_ip_address::local_ip() {
        Ok(ip) => {ip.to_string()}
        Err(__error) => {"No Address Found".to_string()}
    };
    print!("%F{{4}} %f%m%F{{4}}@%f{ip}");
}

fn Get_Main_Disk(partitions: &[sysinfo::Disk]) -> String
{
    for partition in partitions {
        if partition.mount_point() == std::path::PathBuf::from("/") {
            if let Some(name) = partition.name().to_str() {
                let mut characters = name.chars()
                                                    .collect::<Vec<char>>();
                characters.pop();
                return characters.iter().collect();
            }
        }
    }
    Throw_Error("can not get disk usage.");
}

fn Get_Disk_Usage() -> i32
{
    let mut system = sysinfo::System::new();
    system.refresh_disks_list();
    let mut total_size_in_bytes = 0;
    let mut available_size_in_bytes = 0;
    let partitions = system.disks();
    let main_disk = Get_Main_Disk(partitions);
    for partition in partitions {
        if let Some(name) = partition.name().to_str() {
            if name.contains(&main_disk) {
                total_size_in_bytes += partition.total_space();
                available_size_in_bytes += partition.available_space();
            }
        }
    }
    let mut usage_percentage = 0;
    if total_size_in_bytes != 0 {
        let used_size_in_bytes = total_size_in_bytes -
                                      available_size_in_bytes;
        usage_percentage = ((used_size_in_bytes as f32 / total_size_in_bytes as
                             f32) * 100.0) as i32;
    }
    usage_percentage
}

fn Print_Disk_Usage()
{
    print!("%F{{3}}󰋊 %f{}%%", Get_Disk_Usage());
}

fn main()
{
    Print_Commands_Separator();
    print!("%F{{3}}:«");
    Print_Local_Ip_Address();
    Print_Spacing();
    Print_Disk_Usage();
}
