use crossterm::terminal::size;

pub struct TerminalEmulator;

impl TerminalEmulator
{
	pub fn get_columns() -> u16
	{
		match size()
		{
			Ok(size) =>
			{
				size.0
			}
			Err(_error) =>
			{
				0
			}
		}
	}
}

