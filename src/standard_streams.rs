use std::process::exit;
use crossterm::terminal::size;
use super::error_treatment::print_error_message;

pub fn get_output_stream_width() -> u16
{
	match size()
	{
		Ok(size) =>
		{
			let width: u16 = size.0;
			let _height: u16 = size.1;
			width
		}
		Err(_) =>
		{
			print_error_message("Could not get output stream width.");
			exit(1);
		}
	}
}

