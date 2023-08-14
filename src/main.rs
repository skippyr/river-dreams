#![allow(nonstandard_style)]
#![allow(unused_parens)]

use chrono::{Datelike, Timelike};
use sysinfo::{DiskExt, SystemExt};

fn Throw_Error(description: impl std::fmt::Display) -> ! {
    eprintln!("river-dreams: {description}");
    std::process::exit(1);
}

fn Print_Spacing() {
    print!("  ");
}

fn Print_Commands_Separator() {
    let size = crossterm::terminal::size().unwrap_or_else(|__error| {
        Throw_Error("can not get terminal size.");
    });
    for column in 0..size.0 {
        print!("{}%f", if column % 2 == 0 {"%F{3}⊵"} else {"%F{1}⊼"});
    }
}

fn Print_Local_Ip_Address() {
    let ip = match local_ip_address::local_ip() {
        Ok(ip) => {ip.to_string()}
        Err(__error) => {"No Address Found".to_string()}
    };
    print!("%F{{4}} %f%m%F{{4}}@%f{ip}");
}

fn Get_Main_Disk_Name(partitions: &[sysinfo::Disk]) -> String {
    for partition in partitions {
        if partition.mount_point() == std::path::PathBuf::from("/") {
            if let Some(name) = partition.name().to_str() {
                let mut characters = name.chars().collect::<Vec<char>>();
                characters.pop();
                return (characters.iter().collect());
            }
        }
    }
    Throw_Error("can not get main disk name.");
}

fn Get_Disk_Usage() -> u8 {
    let mut system = sysinfo::System::new();
    system.refresh_disks_list();
    let mut total_size = 0;
    let mut available_size = 0;
    let partitions = system.disks();
    let main_disk_name = Get_Main_Disk_Name(partitions);
    for partition in partitions {
        if let Some(name) = partition.name().to_str() {
            if name.contains(&main_disk_name) {
                total_size += partition.total_space();
                available_size += partition.available_space();
            }
        }
    }
    let mut usage = 0;
    if total_size != 0 {
        let used_size = total_size - available_size;
        usage = ((used_size as f32 / total_size as f32) * 100.0) as u8;
    }
    return (usage);
}

fn Print_Disk_Usage() {
    print!("%F{{3}}󰋊 %f{}%%", Get_Disk_Usage());
}

fn Get_Month(moment: &chrono::DateTime<chrono::Local>) -> String {
    return (
        match moment.month0() {
            1 => {"Feb"}
            2 => {"Mar"}
            3 => {"Apr"}
            4 => {"May"}
            5 => {"Jun"}
            6 => {"Jul"}
            7 => {"Aug"}
            8 => {"Sep"}
            9 => {"Oct"}
            10 => {"Nov"}
            _ => {"Jan"}
        }.to_string()
    );
}

fn Get_Ordinal(day: u32) -> String {
    return (
        if day - 1 % 10 == 0 {
            "st"
        } else if day - 2 % 10 == 0 {
            "nd"
        } else if day - 3 % 10 == 0 {
            "rd"
        } else {
            "th"
        }.to_string()
    );
}

fn Print_Calendar(moment: &chrono::DateTime<chrono::Local>) {
    let day = moment.day();
    print!("%F{{1}}󰸗 %f({}) {} {day}{}", moment.weekday(), Get_Month(moment),
           Get_Ordinal(day));
}

fn Get_Clock_Symbol(hour: u32) -> String {
    return (
        if hour < 6 {
            "%F{6}󰭎"
        } else if hour < 12 {
            "%F{1}󰖨  "
        } else if hour < 18 {
            "%F{4} "
        } else {
            "%F{3}󰽥 "
        }.to_string()
    );
}

fn Format_Time(time: u32) -> String {
    return(format!("{}{time}", if time < 10 {"0"} else {""}));
}

fn Print_Clock(moment: &chrono::DateTime<chrono::Local>) {
    let hour = moment.hour();
    print!("{}%f{}h{}m", Get_Clock_Symbol(hour), Format_Time(hour),
           Format_Time(moment.minute()));
}

fn Print_Virtual_Environment() {
    if let Ok(virtual_environment) = std::env::var("VIRTUAL_ENV") {
        let error_description = "can not get virtual environment.";
        let virtual_environment = std::path::PathBuf::from(virtual_environment);
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

fn main() {
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
    print!("%F{{3}}»:\n%(#.{{%F{{1}}#%F{{3}}}}.){{%(?.≗.%F{{1}}⨲%F{{3}})}}⤐  ");
    Print_Virtual_Environment();
    print!("%F{{1}}%1~ %F{{6}}⩺ %f");
}
