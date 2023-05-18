use crate::
{
	environment::EnvironmentVariables,
	styles::Color
};
use std::fmt::
{
	Display,
	Formatter,
	Result
};

pub enum AppearingCondition
{
	None,
	OnRootUser,
	OnJob
}

pub struct PromptString
{
	default_text: String,
	fallback_text: Option<String>,
	appearing_condition: AppearingCondition,
	color: Color
}

impl PromptString
{
	pub fn new<
		A: Display,
		B: Display
	>(
		default_text: A,
		fallback_text: Option<B>,
		appearing_condition: AppearingCondition,
		color: Color
	) -> PromptString
	{
		let default_text: String = format!(
			"{}",
			default_text
		);
		let fallback_text: Option<String> = match fallback_text
		{
			Some(fallback_text) =>
			{
				Some(
					format!(
						"{}",
						fallback_text
					)
				)
			}
			None =>
			{ None }
		};
		PromptString
		{
			default_text,
			fallback_text,
			appearing_condition,
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
		let text: String =
			if EnvironmentVariables::is_to_use_fallback_text()
			{
				match &self.fallback_text
				{
					Some(fallback_text) =>
					{ fallback_text.clone() }
					None =>
					{ self.default_text.clone() }
				}
			}
			else
			{ self.default_text.clone() };
		let colored_text: String = format!(
			"{}{}%f",
			self.color.get_style_sequence(),
			text
		);
		let format: String = match self.appearing_condition
		{
			AppearingCondition::None =>
			{ colored_text }
			AppearingCondition::OnRootUser =>
			{
				format!(
					"%(#.{}.)",
					colored_text
				)
			}
			AppearingCondition::OnJob =>
			{
				format!(
					"%(1j.{}.)",
					colored_text
				)
			}
		};
		write!(
			formatter,
			"{}",
			format
		)
	}
}

