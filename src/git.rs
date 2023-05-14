use std::
{
	fs::
	{
		ReadDir,
		read_dir,
		DirEntry,
		File,
		Metadata
	},
	io::
	{
		BufReader,
		BufRead
	},
	path::PathBuf
};
use crate::file_system::paths::
{
	PathAbbreviation,
	Paths
};

#[derive(Clone)]
pub struct GitBranch
{ name: String }

impl GitBranch
{
	pub fn from_repository_directory(repository_directory: &PathBuf) -> Option<GitBranch>
	{
		let head_file_path: PathBuf = repository_directory
			.clone()
			.join(".git")
			.join("HEAD");
		let head_file: File = match File::open(head_file_path)
		{
			Ok(head_file) =>
			{ head_file }
			Err(_error) =>
			{ return None; }
		};
		let head_file_reader: BufReader<File> = BufReader::new(head_file);
		let mut branch_name: String = String::new();
		for line in head_file_reader.lines()
		{
			let line: String = match line
			{
				Ok(line) =>
				{ line }
				Err(_error) =>
				{ return None; }
			};
			let splits: Vec<&str> = line.split("/").collect();
			if
				splits.len() == 3 &&
				splits[2] != ""
			{ branch_name = String::from(splits[2]); }
			else
			{ return None; }
			break;
		}
		Some(GitBranch { name: branch_name })
	}

	pub fn get_name(&self) -> String
	{ self.name.clone() }
}

pub struct GitRepository
{
	path: PathBuf,
	branch: GitBranch
}

impl GitRepository
{
	fn get_repository_directory(directory_path: PathBuf) -> Option<PathBuf>
	{
		let directory_stream: ReadDir = match read_dir(&directory_path)
		{
			Ok(directory_stream) =>
			{ directory_stream }
			Err(_error) =>
			{ return None; }
		};
		for directory_entry in directory_stream
		{
			let directory_entry: DirEntry = match directory_entry
			{
				Ok(directory_entry) =>
				{ directory_entry }
				Err(_error) =>
				{ continue; }
			};
			let directory_entry_path: PathBuf = directory_entry.path();
			let directory_entry_metadata: Metadata = match directory_entry.metadata()
			{
				Ok(directory_entry_metadata) =>
				{ directory_entry_metadata }
				Err(_error) =>
				{ continue; }
			};
			if let Some(file_name) = directory_entry_path.file_name_as_string()
			{
				if
					file_name == ".git" &&
					directory_entry_metadata.is_dir()
				{ return Some(directory_path); }
			}
		}
		match directory_path.parent()
		{
			Some(parent_directory_path) =>
			{ GitRepository::get_repository_directory(parent_directory_path.to_path_buf()) }
			None =>
			{ None }
		}
	}

	pub fn from_pwd() -> Option<GitRepository>
	{
		let path: PathBuf = match GitRepository::get_repository_directory(Paths::get_pwd())
		{
			Some(path) =>
			{ path }
			None =>
			{ return None; }
		};
		let branch: GitBranch = match GitBranch::from_repository_directory(&path)
		{
			Some(branch) =>
			{ branch }
			None =>
			{ return None; }
		};
		Some(
			GitRepository
			{
				path,
				branch
			}
		)
	}

	pub fn get_branch(&self) -> GitBranch
	{ self.branch.clone() }

	pub fn get_path(&self) -> PathBuf
	{ self.path.clone() }
}

