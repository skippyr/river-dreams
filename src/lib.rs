pub mod error_treatment
{
	pub fn print_error_message(message: &str)
	{
		eprintln!("Opsie!");
		eprintln!(
			"\triver_dreams: {}",
			message
		);
	}
}

pub mod environment
{
	pub fn is_to_use_fallback_text() -> bool
	{
		match option_env!("RIVER_DREAMS_USE_FALLBACK_TEXT")
		{
			Some(is_to_use_fallback_text) =>
			{ is_to_use_fallback_text == "1" }
			None =>
			{ false }
		}
	}
}

pub mod streams
{
	use std::process::exit;
	use crossterm::terminal::size;
	use super::error_treatment::print_error_message;

	pub fn get_standart_output_width() -> u16
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
				print_error_message("Could not get standart output width.");
				exit(1);
			}
		}
	}
}

pub mod styles
{
	pub enum Color
	{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		Normal
	}

	pub fn get_color_as_string(color: Color) -> String
	{
		format!(
			"%F{{{}}}",
			match color
			{
				Color::Black => "black",
				Color::Red => "red",
				Color::Green => "green",
				Color::Yellow => "Yellow",
				Color::Blue => "blue",
				Color::Magenta => "magenta",
				Color::Cyan => "cyan",
				Color::White => "white",
				Color::Normal => "normal"
			}
		)
	}

	pub fn colorize_text(
		text: String,
		color: Color
	) -> String
	{
		format!(
			"{}{}%f",
			get_color_as_string(color),
			text
		)
	}
}

pub mod symbols
{
	use super::environment::is_to_use_fallback_text;

	pub struct Symbol
	{
		pub default: String,
		pub fallback: String
	}

	pub fn get_symbol_by_environment(symbol: Symbol) -> String
	{
		if is_to_use_fallback_text()
		{ symbol.fallback }
		else
		{ symbol.default }
	}
}
