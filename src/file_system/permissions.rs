pub struct UnixPermissions
{ mode: u32 }

impl UnixPermissions
{
	pub fn from(mode: u32) -> UnixPermissions
	{ UnixPermissions { mode } }

	pub fn does_owner_can_execute(&self) -> bool
	{
		const UNIX_OWNER_EXECUTION_PERMISSIONS_BIT: u32 = 0o100;
		self.mode & UNIX_OWNER_EXECUTION_PERMISSIONS_BIT != 0
	}
}

