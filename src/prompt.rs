use crate::
{
	styles::Color,
	environment::EnvironmentAcessor
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
				if EnvironmentAcessor::is_to_use_fallback_text()
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
	{ PromptComponent { structure: String::new() } }

	pub fn append_to_structure(
		&mut self,
		structure_component: String
	)
	{ self.structure.push_str(&structure_component) }

	pub fn get_structure(&self) -> String
	{ self.structure.clone() }
}

pub struct Prompt
{ structure: String }

impl Prompt
{
	pub fn new() -> Prompt
	{ Prompt { structure: String::new() } }

	pub fn append_to_structure(
		&mut self,
		structure_component: String
	)
	{ self.structure.push_str(&structure_component) }
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
			self.structure
		)
	}
}

