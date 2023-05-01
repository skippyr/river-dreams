use super::environment_variables::is_to_use_fallback_text;

pub struct Symbol
{
	pub default: String,
	pub fallback: String
}

pub fn get_symbol_string(symbol: Symbol) -> String
{
	if is_to_use_fallback_text()
	{ symbol.fallback }
	else
	{ symbol.default }
}

