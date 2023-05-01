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

