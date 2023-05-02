use super::error_treatment::print_error_message;
use std::process::exit;
use crossterm::terminal::size;

pub fn get_terminal_width() -> u16
{
	match size()
	{
		Ok(terminal_size) =>
		{
			let terminal_width: u16 = terminal_size.0;
			let _terminal_height: u16 = terminal_size.1;
			terminal_width
		}
		Err(_error) =>
		{
			print_error_message("Could not get output stream width.");
			exit(1);
		}
	}
}

