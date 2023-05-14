use std::
{
	fs::Metadata,
	os::unix::prelude::MetadataExt
};
use users::get_current_uid;
use crate::file_system::paths::Paths;

const ROOT_UID: u32 = 0;

pub struct Users;

impl Users
{
	pub fn is_user_root() -> bool
	{ get_current_uid() == ROOT_UID }

	pub fn get_pwd_owner_uid() -> u32
	{
		let metadata: Metadata = match Paths::get_pwd().metadata()
		{
			Ok(metadata) =>
			{ metadata }
			Err(_error) =>
			{ return ROOT_UID; }
		};
		metadata.uid()
	}
}

