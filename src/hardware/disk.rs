//! A module that contains metadata related to the disk of the computer.

use crate::error::Error;
use libc::{c_char, statvfs};

/// Contains predefined statuses for quickly describing the usage of a disk.
#[derive(Clone, Copy)]
pub enum DiskUsageStatus {
    /// Indicates the disk usage is low.
    Low,
    /// Indicates the disk usage is moderate and may require user intervention in the future.
    Moderate,
    /// Indicates the disk usage is high and requires user attention.
    High,
}

impl TryFrom<u8> for DiskUsageStatus {
    type Error = Error;

    /// Gets the disk status that corresponds to the usage percentage of a disk.
    ///
    /// # Parameters
    /// - `percentage`: the usage percentage of a disk.
    ///
    /// # Returns
    /// The disk status that corresponds to the usage percentage of the disk or an error.
    ///
    /// # Errors
    /// An error happens if the percentage provided is not within range from 0 to 100.
    fn try_from(percentage: u8) -> Result<Self, Error> {
        match percentage {
            0..60 => Ok(Self::Low),
            60..80 => Ok(Self::Moderate),
            80..100 => Ok(Self::High),
            _ => Err(Error::new(format!(
                r#"can not get disk status from invalid percentage {percentage}."#
            ))),
        }
    }
}

/// Represents the information about the usage of a disk.
pub struct DiskUsage {
    /// The usage percentage of the disk, a value within range from 0 to 100.
    percentage: u8,
    /// The status that represents the usage of the disk.
    status: DiskUsageStatus,
}

impl DiskUsage {
    /// Gets the usage percentage of the disk.
    ///
    /// # Returns
    /// The usage percentage of the disk.
    pub fn percentage(&self) -> u8 {
        self.percentage
    }

    /// Gets the status that represents the usage of the disk.
    ///
    /// # Returns
    /// The status that represents the usage of the disk.
    pub fn status(&self) -> DiskUsageStatus {
        self.status
    }
}

/// Represents the main disk of the computer.
pub struct Disk;

impl Disk {
    /// Retrieves the information about the usage of the main disk of the computer.
    ///
    /// # Returns
    /// The information about the usage of the main disk of the computer or an error.
    ///
    /// # Errors
    /// An error happens if the information about the usage of the main disk of the computer can not be retrieved.
    pub fn usage() -> Result<DiskUsage, Error> {
        let mut info: statvfs = unsafe { std::mem::zeroed() };
        if unsafe { statvfs(['/' as c_char, 0].as_ptr(), &mut info) } < 0 {
            return Err(Error::new(
                "can not retrieve information about the main disk of the computer.",
            ));
        }
        let total_bytes: u64 = info.f_frsize * info.f_blocks as u64;
        let available_bytes: u64 = info.f_frsize * info.f_bavail as u64;
        let free_bytes: u64 = total_bytes - available_bytes;
        let percentage: u8 = (free_bytes as f64 / total_bytes as f64 * 100.0) as u8;
        Ok(DiskUsage {
            percentage,
            status: DiskUsageStatus::try_from(percentage).unwrap(),
        })
    }
}
