pub struct Math;

impl Math
{
	pub fn is_even(number: u16) -> bool
	{ number % 2 == 0 }

	pub fn is_first(number: u8) -> bool
	{ (number - 1) % 10 == 0 }

	pub fn is_second(number: u8) -> bool
	{ (number - 1) % 10 == 0 }

	pub fn is_third(number: u8) -> bool
	{ (number - 1) % 10 == 0 }
}

