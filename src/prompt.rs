use crate::
{
	styles::Color,
	environment::EnvironmentVariables
};
use std::fmt::
{
	Display,
	Formatter,
	Result
};

pub struct PromptString
{
	default_content: String,
	fallback_content: Option<String>,
	color: Color
}

impl PromptString
{
	pub fn new(
		default_content: String,
		fallback_content: Option<String>,
		color: Color
	) -> PromptString
	{
		PromptString
		{
			default_content,
			fallback_content,
			color
		}
	}
}

impl Display for PromptString
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		let content: String = match &self.fallback_content
		{
			Some(fallback_content) =>
			{
				if EnvironmentVariables::is_to_use_fallback_text()
				{ fallback_content.clone() }
				else
				{ self.default_content.clone() }
			}
			None =>
			{ self.default_content.clone() }
		};
		write!(
			formatter,
			"{}{}%f",
			self.color.get_escape_sequence(),
			content
		)
	}
}

pub struct PromptComponent
{ structure: String }

impl PromptComponent
{
	pub fn new() -> PromptComponent
	{ PromptComponent {structure: String::new() } }

	pub fn push(
		&mut self,
		part: String
	)
	{ self.structure.push_str(&part); }

	pub fn get_structure(&self) -> String
	{ self.structure.clone() }
}

pub struct Prompt
{ components: Vec<PromptComponent> }

impl Prompt
{
	pub fn new() -> Prompt
	{ Prompt { components: Vec::new() } }

	pub fn push(
		&mut self,
		component: PromptComponent
	)
	{ self.components.push(component); }

	fn get_structure(&self) -> String
	{
		let mut structure: String = String::new();
		for component in &self.components
		{ structure.push_str(&component.get_structure()) }
		structure
	}
}

impl Display for Prompt
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		write!(
			formatter,
			"{}",
			self.get_structure()
		)
	}
}

pub struct ErrorString
{ content: String }

impl ErrorString
{
	pub fn new(content: String) -> ErrorString
	{ ErrorString { content } }
}

impl Display for ErrorString
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		write!(
			formatter,
			"%(?..{})",
			self.content
		)
	}
}

