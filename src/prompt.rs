
pub struct PromptComponent
{ body: String }

impl PromptComponent
{
	pub fn new(body: String) -> PromptComponent
	{ PromptComponent { body } }

	pub fn get_body(&self) -> String
	{ self.body.clone() }
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
				{ prompt.push_str(&component.get_body()) }
			);
		prompt
	}
}

