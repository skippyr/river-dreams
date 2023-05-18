use crate::environment::EnvironmentVariables;
use std::path::PathBuf;

pub struct Paths;

impl Paths
{
	pub fn get_current_directory() -> PathBuf
	{ PathBuf::from(EnvironmentVariables::get_pwd()) }
}

