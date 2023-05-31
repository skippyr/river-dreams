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
	Default,
	OnRootUser,
	OnJob,
	OnError,
	OnSuccess
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
		GenericType0: Display,
		GenericType1: Display
	>(
		default_text: GenericType0,
		fallback_text: Option<GenericType1>,
		appearing_condition: AppearingCondition,
		color: Color
	) -> Self
	{
		let default_text: String = format!(
			"{}",
			default_text
		);
		let fallback_text: Option<String> =
			match fallback_text
			{
				Some(fallback_text) =>
				{
					Some(format!(
						"{}",
						fallback_text
					))
				}
				None =>
				{
					None
				}
			};
		Self {
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
		let text: String = if EnvironmentVariables::is_to_use_fallback_text()
		{
			match &self.fallback_text
			{
				Some(fallback_text) =>
				{
					fallback_text.clone()
				}
				None =>
				{
					self.default_text.clone()
				}
			}
		}
		else
		{
			self.default_text.clone()
		};
		let colored_text: String = format!(
			"{}{}%f",
			self.color.get_style_sequence(),
			text
		);
		let format: String = match self.appearing_condition
		{
			AppearingCondition::Default =>
			{
				colored_text
			}
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
			},
			AppearingCondition::OnError =>
			{
				format!(
					"%(?..{})",
					colored_text
				)
			},
			AppearingCondition::OnSuccess =>
			{
				format!(
					"%(?.{}.)",
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

pub struct PromptComponent
{
	structure: String
}

impl PromptComponent
{
	pub fn new() -> Self
	{
		Self {
			structure: String::new()
		}
	}

	pub fn from<GenericType0: Display>(part: GenericType0) -> Self
	{
		let part: String = format!(
			"{}",
			part
		);
		Self {
			structure: part
		}
	}

	pub fn push<GenericType0: Display>(
		&mut self,
		part: GenericType0
	)
	{
		let part: String = format!(
			"{}",
			part
		);
		self.structure.push_str(&part);
	}

	pub fn get_structure(&self) -> String
	{
		self.structure.clone()
	}
}

pub struct Prompt
{
	components: Vec<PromptComponent>
}

impl Prompt
{
	pub fn new() -> Self
	{
		Self {
			components: Vec::new()
		}
	}

	pub fn push(
		&mut self,
		component: PromptComponent
	)
	{
		self.components.push(component);
	}
}

impl Display for Prompt
{
	fn fmt(
		&self,
		formatter: &mut Formatter
	) -> Result
	{
		let mut format: String = String::new();
		for component in &self.components
		{
			format.push_str(&component.get_structure());
		}
		write!(
			formatter,
			"{}",
			format
		)
	}
}

