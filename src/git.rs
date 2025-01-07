//! A module that contains attributes and functionalities related to Git repositories.

use git2::{
    Repository as Git2Repository, RepositoryState as Git2RepositoryState, Status as Git2Status,
};
use std::path::PathBuf;

/// The configuration key refered by Git to store the repository default branch.
const DEFAULT_BRANCH_KEY: &str = "init.defaultBranch";
/// The default branch name used by Git when no configuration has ever been set.
const DEFAULT_BRANCH_NAME: &str = "master";

/// Contains the possible types of references used to identify a commit.
pub enum GitReference {
    /// Represents a branch.
    Branch(
        /// The name of the branch.
        String,
    ),
    /// Represents a commit hash, used in case of rebase conflicts.
    Hash(
        /// The hash that represents the commit in its abbreviated form of 7 graphemes.
        String,
    ),
}

/// Represents the information about a Git repository.
pub struct GitRepository {
    /// The path to the repository.
    path: PathBuf,
    /// The current reference of the repository.
    reference: GitReference,
    /// A boolean that states the repository has changes to be commited.
    has_changes: bool,
}

impl GitRepository {
    /// Gets the path to the repository.
    ///
    /// # Returns
    /// The path to the repository.
    pub fn path(&self) -> &PathBuf {
        &self.path
    }

    /// Gets the current reference of the repository.
    ///
    /// # Returns
    /// The current reference of the repository.
    pub fn reference(&self) -> &GitReference {
        &self.reference
    }

    /// Checks whether the repository has changes to be commited.
    ///
    /// # Returns
    /// A boolean that states the repository has changes to be commited.
    pub fn has_changes(&self) -> bool {
        self.has_changes
    }
}

impl GitRepository {
    /// Searches recursively for a Git repository starting from the current directory until the root of the file system.
    ///
    /// # Returns
    /// The repository found or nothing.
    pub fn search_from_current_directory() -> Option<GitRepository> {
        let repository: Git2Repository = match Git2Repository::discover(".") {
            Ok(repository) => repository,
            Err(_) => return None,
        };
        let mut reference: Option<GitReference> = None;
        if let Ok(head) = repository.head() {
            if repository.state() == Git2RepositoryState::RebaseInteractive {
                if let Some(hash) = head.target() {
                    reference = Some(GitReference::Hash(
                        hash.to_string().chars().take(7).collect::<String>(),
                    ));
                }
            } else if let Some(branch) = head.shorthand() {
                reference = Some(GitReference::Branch(String::from(branch)));
            }
        } else {
            if let Ok(head_data) = std::fs::read_to_string(repository.path().join("HEAD")) {
                if let Some(branch) = head_data.trim().strip_prefix("ref: refs/heads/") {
                    reference = Some(GitReference::Branch(branch.to_string()));
                }
            } else if let Ok(configuration) = repository.config() {
                if let Ok(branch) = configuration.get_string(DEFAULT_BRANCH_KEY) {
                    reference = Some(GitReference::Branch(branch));
                }
            }
        }
        let mut has_changes: bool = false;
        if let Ok(statuses) = repository.statuses(None) {
            for entry in &statuses {
                if entry.status().contains(Git2Status::IGNORED) {
                    continue;
                } else if entry.status().intersects(
                    Git2Status::INDEX_NEW
                        | Git2Status::INDEX_MODIFIED
                        | Git2Status::INDEX_DELETED
                        | Git2Status::INDEX_RENAMED
                        | Git2Status::INDEX_TYPECHANGE
                        | Git2Status::WT_NEW
                        | Git2Status::WT_MODIFIED
                        | Git2Status::WT_DELETED
                        | Git2Status::WT_TYPECHANGE
                        | Git2Status::WT_RENAMED,
                ) {
                    has_changes = true;
                    break;
                }
            }
        }
        Some(GitRepository {
            path: if repository.is_bare() {
                repository.path().to_path_buf()
            } else {
                repository.workdir().unwrap().to_path_buf()
            },
            reference: match reference {
                Some(reference) => reference,
                None => GitReference::Branch(String::from(DEFAULT_BRANCH_NAME)),
            },
            has_changes,
        })
    }
}
