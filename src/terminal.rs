use super::error_treatment::print_error_message;
use std::process::exit;
use crossterm::terminal::size;

pub struct Terminal
{
	columns: u16,
	lines: u16
}

impl Terminal
{
	pub fn new() -> Terminal
	{
		let (
			columns,
			lines
		) = match size()
		{
			Ok(size) =>
			{ size }
			Err(_error) =>
			{
				print_error_message("Could not get output stream width.");
				exit(1);
			}
		};
		Terminal
		{
			columns,
			lines
		}
	}
	
	pub fn get_columns(&self) -> u16
	{ self.columns }

	pub fn get_lines(&self) -> u16
	{ self.lines }
}

