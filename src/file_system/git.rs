use std::
{
	fs::
	{
		ReadDir,
		DirEntry,
		read_dir,
		FileType,
		File
	},
	path::PathBuf,
	io::
	{
		BufReader,
		BufRead
	}
};
use crate::file_system::paths::Paths;

#[derive(Clone)]
pub struct Branch
{ name: String }

impl Branch
{
	pub fn from(path: &PathBuf) -> Option<Self>
	{
		let head_file: File = match File::open(path.join(".git").join("HEAD"))
		{
			Ok(file) =>
			{ file }
			Err(_error) =>
			{ return None; }
		};
		let reader: BufReader<File> = BufReader::new(head_file);
		for line in reader.lines()
		{
			let line: String = match line
			{
				Ok(line) =>
				{ line }
				Err(_error) =>
				{ return None; }
			};
			let splits: Vec<&str> = line.split("/").collect();
			if splits.len() < 3
			{ return None; }
			let name: String = String::from(splits[2]);
			return Some(Branch { name });
		}
		None
	}

	pub fn get_name(&self) -> String
	{ self.name.clone() }
}

pub struct Repository
{
	path: PathBuf,
	branch: Branch
}

impl Repository
{
	fn get_repository_path(path: &PathBuf) -> Option<PathBuf>
	{
		let stream: ReadDir = match read_dir(&path)
		{
			Ok(stream) =>
			{ stream } 
			Err(_error) =>
			{ return None; }
		};
		for entry in stream
		{
			let entry: DirEntry = match entry
			{
				Ok(entry) =>
				{ entry }
				Err(_error) =>
				{ return None; }
			};
			let name: String = match entry.file_name().to_str()
			{
				Some(name) =>
				{ String::from(name) }
				None =>
				{ continue; }
			};
			let file_type: FileType = match entry.file_type()
			{
				Ok(file_type) =>
				{ file_type }
				Err(_error) =>
				{ continue; }
			};
			if
				name == ".git" &&
				file_type.is_dir()
			{ return Some(path.clone()); }
		}
		match path.parent()
		{
			Some(parent) =>
			{ Self::get_repository_path(&parent.to_path_buf()) }
			None =>
			{ None }
		}
	}

	pub fn from_current_directory() -> Option<Self>
	{
		let path: PathBuf = match Self::get_repository_path(&Paths::get_current_directory())
		{
			Some(path) =>
			{ path }
			None =>
			{ return None; }
		};
		let branch: Branch = match Branch::from(&path)
		{
			Some(branch) =>
			{ branch }
			None =>
			{ return None; }
		};
		Some(Self {
			path,
			branch
		})
	}

	pub fn get_branch(&self) -> Branch
	{ self.branch.clone() }

	pub fn get_path(&self) -> PathBuf
	{ self.path.clone() }
}

