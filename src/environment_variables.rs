use std::env::var;

pub fn is_to_use_fallback_text() -> bool
{
	match var("RIVER_DREAMS_USE_FALLBACK_TEXT")
	{
		Ok(is_to_use_fallback_text) =>
		{ is_to_use_fallback_text == "1" }
		Err(_error) =>
		{ false }
	}
}

