//! A module to work with Git repositories.

use crate::file_system::paths::Paths;
use std::{
    fs::{read_dir, DirEntry, File, FileType, ReadDir},
    io::{BufRead, BufReader},
    path::PathBuf,
};

// An abstraction of a Git branch.
#[derive(Clone)]
pub struct Branch {
    name: String,
}

impl Branch {
    /// Returns a possible branch found from a directory path where the
    /// `.git` directory is stored at. It returns `None` if it can not complete
    /// all operations required to parse the `.git/HEAD` file.
    pub fn from(path: &PathBuf) -> Option<Self> {
        let head_file: File = match File::open(path.join(".git").join("HEAD")) {
            Ok(file) => file,
            Err(_error) => {
                return None;
            }
        };
        let reader: BufReader<File> = BufReader::new(head_file);
        for line in reader.lines() {
            let line: String = match line {
                Ok(line) => line,
                Err(_error) => {
                    return None;
                }
            };
            let splits: Vec<&str> = line.split("/").collect();
            if splits.len() < 3 {
                return None;
            }
            let name: String = String::from(splits[2]);
            return Some(Branch { name });
        }
        None
    }

    /// Returns the name of the branch.
    pub fn get_name(&self) -> String {
        self.name.clone()
    }
}

/// An abstraction of a Git repository.
pub struct Repository {
    /// The root path of the repository, where the `.git` directory is stored
    /// at.
    path: PathBuf,
    /// The active branch in the repository.
    branch: Branch,
}

impl Repository {
    /// Searches recursively for a possible Git repository within the directory
    /// path given as paremeter and its parents and returns its root
    /// directory path. It returns `None` if it reaches the root directory
    /// of the file system: `/`.
    fn get_repository_path(path: &PathBuf) -> Option<PathBuf> {
        let stream: ReadDir = match read_dir(&path) {
            Ok(stream) => stream,
            Err(_error) => {
                return None;
            }
        };
        for entry in stream {
            let entry: DirEntry = match entry {
                Ok(entry) => entry,
                Err(_error) => {
                    return None;
                }
            };
            let name: String = match entry.file_name().to_str() {
                Some(name) => String::from(name),
                None => {
                    continue;
                }
            };
            let file_type: FileType = match entry.file_type() {
                Ok(file_type) => file_type,
                Err(_error) => {
                    continue;
                }
            };
            if name == ".git" && file_type.is_dir() {
                return Some(path.clone());
            }
        }
        match path.parent() {
            Some(parent) => Self::get_repository_path(&parent.to_path_buf()),
            None => None,
        }
    }

    /// Returns a possible Git repository found from within the current
    /// directory and its parents. It returns `None` if it neither can found
    /// the repository or parse its branch.
    pub fn from_current_directory() -> Option<Self> {
        let path: PathBuf = match Self::get_repository_path(&Paths::get_current_directory()) {
            Some(path) => path,
            None => {
                return None;
            }
        };
        let branch: Branch = match Branch::from(&path) {
            Some(branch) => branch,
            None => {
                return None;
            }
        };
        Some(Self { path, branch })
    }

    /// Returns the active branch of the repository.
    pub fn get_branch(&self) -> Branch {
        self.branch.clone()
    }

    /// Returns the root directory path of the repository.
    pub fn get_path(&self) -> PathBuf {
        self.path.clone()
    }
}
