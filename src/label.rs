use super::
{
	styles::
	{
		Color,
		colorize
	},
	environment_variables::is_to_use_fallback_text
};

pub struct Label
{
	default_text: String,
	fallback_text: String,
	color: Color
}

impl Label
{
	pub fn new() -> Label
	{
		Label
		{
			default_text: String::new(),
			fallback_text: String::new(),
			color: Color::Default
		}
	}

	pub fn set_default_text(
		&mut self,
		default_text: String
	)
	{ self.default_text = default_text; }

	pub fn set_fallback_text(
		&mut self,
		fallback_text: String
	)
	{ self.fallback_text = fallback_text; }

	pub fn set_color(
		&mut self,
		color: Color
	)
	{ self.color = color; }

	pub fn as_string(&self) -> String
	{
		let text: String = if is_to_use_fallback_text()
		{ self.fallback_text.clone() }
		else
		{ self.default_text.clone() };
		colorize(
			text,
			self.color.clone()
		)
	}
}
