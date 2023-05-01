use super::error_treatment::print_error_message;
use std::process::exit;
use crossterm::terminal::size;

pub fn get_output_stream_width() -> u16
{
	match size()
	{
		Ok(output_stream_size) =>
		{
			let output_stream_width: u16 = output_stream_size.0;
			let _output_stream_height: u16 = output_stream_size.1;
			output_stream_width
		}
		Err(_error) =>
		{
			print_error_message("Could not get output stream width.");
			exit(1);
		}
	}
}

