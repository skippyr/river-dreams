//! # About
//! River Dreams is a tribal looking theme for the ZSH shell made to help you craft your most ambitious software projects. It is available for macOS and Linux.
//!
//! This library crate contains all of its components and functionalities, intended for people who want to develop it.
//!
//! If you want to install it, instead, follow the instructions in its [repository](https://github.com/skippyr/river_dreams).
//!
//! ## Prompts & Modules
//! The theme is composed of two prompts, one in the left and other in the right. Each prompt has modules that provides its features. From left to right, top to bottom, those are:
//!
//! ### Left Prompt Modules
//! The left prompt prompt modules are:
//!
//! - **IP Address Module:** displays the IP address of the computer.
//! - **Battery Module:** which, in case of a laptop, displays the information about the charge of the battery of the computer, including its percentage and status.
//! - **Disk Module:** which displays the information about the usage of the main disk of the computer, including its percentage and status.
//! - **Calendar Module:** which displays a calendar showing the week day, month, and day of month.
//! - **Clock Module:** which display a 24-hours clock with a symbol that changes accordingly to the day fraction.
//! - **Root Module:** which displays a hash symbol when the user is `root`.
//! - **Exit Code Module:** which displays the exit code of the last command inserted in the shell.
//! - **Virtual Env Module:** which displays the active Python virtual environment sourced.
//! - **Path Module:** which displays the current directory path, abbreviating it when inside of Git repositories.
//! - **Git Module:** which displays information about an active repository, including the active reference branch name or hash and a ballot X symbol when there are changes to be commited.
//! - **Directory Access Module:** which displays a lock symbol whenever the user can not write to the current directory.
//!
//! ### Right Prompt Modules
//! The right prompt prompt modules are:
//!
//! - **Directory Summary Module:** which displays the total number of each entry type in the current directory.
//! - **Jobs Module:** which displays the total number of background jobs running in the shell.
//!
//! ### Note for Developers
//!
//! In Rust, each prompt receives its own implementation within the `prompt` module. All of their functionalities and shell integrations and implemented separately thoughout the remaining modules.
//!
//! # Help
//! If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/river-dreams/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.
//!
//! # Contributing
//! This project is open to review and possibly accept contributions in the form of bug reports and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/river-dreams/pulls) or to my [e-mail](mailto:skippyr.developer@icloud.com).
//!
//! # License
//! This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

pub mod arguments;
pub mod error;
pub mod file_system;
pub mod format;
pub mod git;
pub mod hardware;
pub mod metadata;
pub mod operating_system;
pub mod prompt;
pub mod shell;
pub mod time;
