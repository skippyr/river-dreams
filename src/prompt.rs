
pub struct PromptComponent
{ structure: String }

impl PromptComponent
{
	pub fn from(structure: String) -> PromptComponent
	{ PromptComponent { structure } }

	pub fn get_structure(&self) -> String
	{ self.structure.clone() }
}

pub struct Prompt
{ components: Vec<PromptComponent> }

impl Prompt
{
	pub fn new() -> Prompt
	{ Prompt { components: Vec::new() } }

	pub fn add_component(
		&mut self,
		prompt_component: PromptComponent
	)
	{ self.components.push(prompt_component); }

	pub fn as_string(&self) -> String
	{
		let mut prompt: String = String::new();
		self
			.components
			.iter()
			.for_each(
				|component|
				{ prompt.push_str(&component.get_structure()) }
			);
		prompt
	}
}

