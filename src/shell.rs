use super::
{
	symbols::Symbol,
	styles::
	{
		Color,
		colorize
	}
};

pub fn get_exit_code_string(error_symbol: &Symbol) -> String
{
	format!(
		"%(?..{}{} %?{})",
		colorize(
			String::from("{"),
			Color::Yellow
		),
		colorize(
			error_symbol.get_text_for_environment(),
			error_symbol.color.clone()
		),
		colorize(
			String::from("}"),
			Color::Yellow
		),
	)
}

pub fn get_user() -> String
{
	String::from("%n")
}
