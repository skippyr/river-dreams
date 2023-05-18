use std::path::PathBuf;

pub struct Branch
{ name: String }

pub struct Repository
{
	path: PathBuf,
	branch: Branch
}

