use crossterm::terminal::size;

pub struct TerminalEmulator
{ columns: u16 }

impl TerminalEmulator
{
	pub fn from_environment() -> TerminalEmulator
	{
		let mut columns: u16 = 0;
		if let Ok(size) = size()
		{ columns = size.0; }
		TerminalEmulator { columns }
	}

	pub fn get_columns(&self) -> u16
	{ self.columns }
}

