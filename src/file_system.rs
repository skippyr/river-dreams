//! A module that contains functionalities related to the file system.

use crate::error::Error;
use libc::{
    access, c_char, closedir, dirent, opendir, readdir, strlen, DIR, DT_BLK, DT_CHR, DT_DIR,
    DT_FIFO, DT_LNK, DT_SOCK, W_OK,
};
#[cfg(target_os = "macos")]
use libc::{lstat, stat, UF_HIDDEN};
use std::path::PathBuf;

/// Contains the possible types for a directory entry.
pub enum EntryType {
    /// The directory type.
    Directory,
    /// The regular file type.
    File,
    /// The socket type.
    Socket,
    /// The fifo type.
    Fifo,
    /// The block device type.
    Block,
    /// The character device type.
    Character,
    /// The symlink type.
    Symlink,
}

/// Contains the total number of each entry type in a directory.
#[derive(Default)]
pub struct DirectoryEntryTotals {
    /// The total number of directory entries found.
    total_directories: usize,
    /// The total number of regular file entries found.
    total_files: usize,
    /// The total number of socket entries found.
    total_sockets: usize,
    /// The total number of fifo entries found.
    total_fifos: usize,
    /// The total number of block device entries found.
    total_blocks: usize,
    /// The total number of character device entries found.
    total_characters: usize,
    /// the total number of symlink entries found.
    total_symlinks: usize,
    /// The total number of hidden entries found.
    total_hiddens: usize,
    /// The total number of temporary entries found.
    total_temporaries: usize,
}

impl DirectoryEntryTotals {
    /// Gets the total number of each entry type in the current directory.
    ///
    /// # Returns
    /// The total number of each entry type in the current directory or an error.
    ///
    /// # Errors
    /// An error happens if the current directory can not be accessed.
    pub fn from_current_directory() -> Result<DirectoryEntryTotals, Error> {
        let stream: *mut DIR = unsafe { opendir(['.' as c_char, 0].as_ptr()) };
        let mut entry_totals: DirectoryEntryTotals = DirectoryEntryTotals::default();
        if stream.is_null() {
            return Err(Error::new("can not access the current directory."));
        }
        loop {
            let entry: *const dirent = unsafe { readdir(stream) };
            if entry.is_null() {
                break;
            }
            let entry: dirent = unsafe { *entry };
            if is_default_dot_directory(&entry) {
                continue;
            } else if is_temporary_entry(&entry) {
                entry_totals.total_temporaries += 1;
            }
            match is_hidden_entry(&entry) {
                Ok(is_hidden) => {
                    if is_hidden {
                        entry_totals.total_hiddens += 1;
                    }
                }
                Err(_) => continue,
            }
            match entry_type(&entry) {
                EntryType::Directory => entry_totals.total_directories += 1,
                EntryType::Socket => entry_totals.total_sockets += 1,
                EntryType::Fifo => entry_totals.total_fifos += 1,
                EntryType::Block => entry_totals.total_blocks += 1,
                EntryType::Character => entry_totals.total_characters += 1,
                EntryType::Symlink => entry_totals.total_symlinks += 1,
                EntryType::File => entry_totals.total_files += 1,
            }
        }
        unsafe {
            closedir(stream);
        }
        Ok(entry_totals)
    }

    /// Gets the total directory entries found.
    ///
    /// # Returns
    /// The total directory entries found.
    pub fn total_directories(&self) -> usize {
        self.total_directories
    }

    /// Gets the total number of regular file entries found.
    ///
    /// # Returns
    /// The total number of regular file entries found.
    pub fn total_files(&self) -> usize {
        self.total_files
    }

    /// Gets the total number of socket entries found.
    ///
    /// # Returns
    /// The total number of socket entries found.
    pub fn total_sockets(&self) -> usize {
        self.total_sockets
    }

    /// Gets the total number of fifo entries found.
    ///
    /// # Returns
    /// The total number of fifo entries found.
    pub fn total_fifos(&self) -> usize {
        self.total_fifos
    }

    /// Gets the total number of block device entries found.
    ///
    /// # Returns
    /// The total number of block device entries found.
    pub fn total_blocks(&self) -> usize {
        self.total_blocks
    }

    /// Gets the total number of character device entries found.
    ///
    /// # Returns
    /// The total number of character device entries found.
    pub fn total_characters(&self) -> usize {
        self.total_characters
    }

    /// Gets the total number of symlink entries found.
    ///
    /// # Returns
    /// The total number of symlink entries found.
    pub fn total_symlinks(&self) -> usize {
        self.total_symlinks
    }

    /// Gets the total number of hidden entries found.
    ///
    /// # Returns
    /// The total number of hidden entries found.
    pub fn total_hiddens(&self) -> usize {
        self.total_hiddens
    }

    /// Gets the total number of temporary entries found.
    ///
    /// # Returns
    /// The total number of temporary entries found.
    pub fn total_temporaries(&self) -> usize {
        self.total_temporaries
    }
}

/// Checks whether a path is the file system root.
///
/// # Parameter
/// - `path`: the path to be checked.
///
/// # Returns
/// A boolean that states the path is the file system root.
pub fn is_file_system_root(path: &PathBuf) -> bool {
    path.ancestors().count() == 1
}

/// Checks whether the user can write to the current directory.
///
/// # Returns
/// A boolean that states the user can write to the current directory.
pub fn can_write_to_current_directory() -> bool {
    unsafe { access(['.' as c_char, 0].as_ptr(), W_OK) == 0 }
}

/// Checks whether a directory entry is a default dot directory: "." or "..".
///
/// # Parameters
/// - `entry`: the entry to be checked.
///
/// # Returns
/// A boolean that states the entry is a default dot directory.
fn is_default_dot_directory(entry: &dirent) -> bool {
    (entry.d_name[0] == '.' as c_char && entry.d_name[1] == 0)
        || (entry.d_name[0] == '.' as c_char
            && entry.d_name[1] == '.' as c_char
            && entry.d_name[2] == 0)
}

/// Checks whether a directory entry is a temporary entry.
///
/// # Parameters
/// - `entry`: the entry to be checked.
///
/// # Returns
/// A boolean that states the entry is a temporary one.
fn is_temporary_entry(entry: &dirent) -> bool {
    entry.d_name[unsafe { strlen(entry.d_name.as_ptr()) } - 1] == '~' as c_char
}

/// Checks whether a directory entry is hidden.
///
/// # Parameters
/// - `entry`: the entry to be checked.
///
/// # Returns
/// A boolean that states the entry is hidden or an error.
fn is_hidden_entry(entry: &dirent) -> Result<bool, ()> {
    let starts_with_dot: bool = entry.d_name.starts_with(&['.' as c_char]);
    if starts_with_dot {
        return Ok(true);
    }
    #[cfg(target_os = "macos")]
    match entry_lstat(entry) {
        Ok(stat) => stat.st_flags & UF_HIDDEN != 0,
        Err(_) => return Err(()),
    };
    Ok(false)
}

/// Gets the status of a directory entry without following it if it is a symlink.
///
/// # Parameters
/// - `entry`: the entry to be checked.
///
/// # Returns
/// The status or an error.
///
/// # Errors
/// An error happens if the status of the entry can not be find.
#[cfg(target_os = "macos")]
fn entry_lstat(entry: &dirent) -> Result<stat, ()> {
    let mut status: stat = unsafe { std::mem::zeroed() };
    if unsafe { lstat(entry.d_name.as_ptr(), &mut status) } < 0 {
        Err(())
    } else {
        Ok(status)
    }
}

/// Gets the type of a directory entry.
///
/// # Parameters
/// - `entry`: the entry to be checked.
///
/// # Returns
/// The type of the entry.
fn entry_type(entry: &dirent) -> EntryType {
    match entry.d_type {
        DT_DIR => EntryType::Directory,
        DT_SOCK => EntryType::Socket,
        DT_FIFO => EntryType::Fifo,
        DT_BLK => EntryType::Block,
        DT_CHR => EntryType::Character,
        DT_LNK => EntryType::Symlink,
        _ => EntryType::File,
    }
}
