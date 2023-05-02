
pub struct PromptComponent
{ structure: String }

impl PromptComponent
{
	pub fn new() -> PromptComponent
	{ PromptComponent { structure: String::new() } }

	pub fn append_string_to_structure(
		&mut self,
		string: String
	)
	{ self.structure.push_str(&string); }

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

