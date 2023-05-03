use super::error_treatment::print_error;
use std::process::exit;
use crossterm::terminal::size;

pub struct Terminal
{ columns: u16 }

impl Terminal
{
	pub fn new() -> Terminal
	{
		let (
			columns,
			_lines
		) = match size()
		{
			Ok(size) =>
			{ size }
			Err(_error) =>
			{
				print_error("could not get terminal size.");
				exit(1);
			}
		};
		Terminal
		{ columns }
	}
	
	pub fn get_columns(&self) -> u16
	{ self.columns }
}

