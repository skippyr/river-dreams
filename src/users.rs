use std::
{
	path::PathBuf,
	os::unix::prelude::MetadataExt
};
use users::get_current_uid;
use crate::file_system::paths::Paths;

pub struct UnixUsers;

impl UnixUsers
{
	pub fn get_owner_uid_of_pwd() -> u32
	{
		let pwd: PathBuf = Paths::get_pwd();
		let metadata = match pwd.metadata()
		{
			Ok(metadata) =>
			{ metadata }
			Err(_error) =>
			{ return 0; }
		};
		metadata.uid()
	}

	pub fn get_current_user_uid() -> u32
	{ get_current_uid() }
}

