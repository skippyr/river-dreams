use super::
{
	styles::
	{
		Color,
		colorize
	},
	environment_variables::is_to_use_fallback_text
};

pub struct TextWithFallback
{
	default_content: String,
	fallback_content: String,
	color: Color
}

impl TextWithFallback
{
	pub fn new() -> TextWithFallback
	{
		TextWithFallback
		{
			default_content: String::new(),
			fallback_content: String::new(),
			color: Color::Default
		}
	}

	pub fn set_default_content(
		&mut self,
		default_content: String
	)
	{ self.default_content = default_content; }

	pub fn set_fallback_content(
		&mut self,
		fallback_content: String
	)
	{ self.fallback_content = fallback_content; }

	pub fn set_color(
		&mut self,
		color: Color
	)
	{ self.color = color; }

	pub fn as_string(&self) -> String
	{
		let content: String = if is_to_use_fallback_text()
		{ self.fallback_content.clone() }
		else
		{ self.default_content.clone() };
		colorize(
			content,
			self.color.clone()
		)
	}
}

pub struct Text
{
	content: String,
	color: Color
}

impl Text
{
	pub fn new() -> Text
	{
		Text
		{
			content: String::new(),
			color: Color::Default
		}
	}

	pub fn set_content(
		&mut self,
		content: String
	)
	{ self.content = content; }

	pub fn set_color(
		&mut self,
		color: Color
	)
	{ self.color = color; }

	pub fn as_string(&self) -> String
	{
		colorize(
			self.content.clone(),
			self.color.clone()
		)
	}

	pub fn as_error_string(&self) -> String
	{
		format!(
			"%(?..{})",
			self.as_string()
		)
	}
}

