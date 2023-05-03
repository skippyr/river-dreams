use super::file_system::
{
	get_pwd_as_path_buff,
	PathAbbreviation
};
use std::
{
	fs::
	{
		ReadDir,
		read_dir,
		DirEntry,
		File
	},
	io::
	{
		BufReader,
		BufRead
	},
	path::PathBuf
};

#[derive(Clone)]
pub struct GitBranch
{ name: String }

impl GitBranch
{
	pub fn from_dot_git_directory(dot_git_directory_path: &PathBuf) -> Option<GitBranch>
	{
		let mut head_file_path: PathBuf = dot_git_directory_path.clone();
		head_file_path.push("HEAD");
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
{ branch: GitBranch }

impl GitRepository
{
	fn get_dot_git_directory_path(directory_path: PathBuf) -> Option<PathBuf>
	{
		let directory_stream: ReadDir = match read_dir(&directory_path)
		{
			Ok(directory_stream) =>
			{ directory_stream }
			Err(_error) =>
			{ return None; }
		};
		let mut dot_git_directory_path: Option<PathBuf> = None;
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
			if let Some(file_name) = directory_entry_path.file_name_as_string()
			{
				if file_name == String::from(".git")
				{ dot_git_directory_path = Some(directory_entry_path); }
			}
		}
		match dot_git_directory_path
		{
			Some(dot_git_directory_path) =>
			{ Some(dot_git_directory_path) }
			None =>
			{
				match directory_path.parent()
				{
					Some(parent_directory_path) =>
					{ return GitRepository::get_dot_git_directory_path(parent_directory_path.to_owned()); }
					None =>
					{ return None; }
				};
			}
		}
	}

	pub fn from_pwd() -> Option<GitRepository>
	{
		let dot_git_directory_path: PathBuf = match GitRepository::get_dot_git_directory_path(get_pwd_as_path_buff())
		{
			Some(dot_git_directory_path) =>
			{ dot_git_directory_path }
			None =>
			{ return None; }
		};
		let branch: GitBranch = match GitBranch::from_dot_git_directory(&dot_git_directory_path)
		{
			Some(branch) =>
			{ branch }
			None =>
			{ return None; }
		};
		Some(GitRepository { branch })
	}

	pub fn get_branch(&self) -> GitBranch
	{ self.branch.clone() }
}

