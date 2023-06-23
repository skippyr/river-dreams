//! A module to work with prompt structures.

use crate::{environment::EnvironmentVariables, styles::Color};
use std::fmt::{Display, Formatter, Result};

/// A set of conditions that a string might appear in the prompt.
pub enum AppearingCondition {
    /// Always visible.
    Default,
    /// Only appears if the user is `root`.
    OnRootUser,
    /// Only appears if there is, at least, one job running in the background.
    OnJob,
    /// Only appears if the last command failed.
    OnError,
    /// Only appears if the last command succeded.
    OnSuccess,
}

/// An abstraction of a string to be used in the prompt.
pub struct PromptString {
    /// The text the string holds by default.
    default_text: String,
    /// A possible fallback text that the string will admit if the fallback
    /// feature is enabled. If `None`, it will use the same as the
    /// `default_text` property.
    fallback_text: Option<String>,
    /// The condition the string will appear on the prompt.
    appearing_condition: AppearingCondition,
    /// The color of the string.
    color: Color,
}

impl PromptString {
    /// Creates a new instance of `PromptString`.
    pub fn new<A: Display, B: Display>(
        default_text: A,
        fallback_text: Option<B>,
        appearing_condition: AppearingCondition,
        color: Color,
    ) -> Self {
        let default_text: String = default_text.to_string();
        let fallback_text: Option<String> = match fallback_text {
            Some(fallback_text) => Some(fallback_text.to_string()),
            None => None,
        };
        Self {
            default_text,
            fallback_text,
            appearing_condition,
            color,
        }
    }
}

impl Display for PromptString {
    fn fmt(&self, formatter: &mut Formatter) -> Result {
        let text: String = if EnvironmentVariables::is_to_use_fallback_text() {
            match &self.fallback_text {
                Some(fallback_text) => fallback_text.clone(),
                None => self.default_text.clone(),
            }
        } else {
            self.default_text.clone()
        };
        let colored_text: String = format!("{}{}%f", self.color.get_style_sequence(), text);
        let format: String = match self.appearing_condition {
            AppearingCondition::Default => colored_text,
            AppearingCondition::OnRootUser => {
                format!("%(#.{}.)", colored_text)
            }
            AppearingCondition::OnJob => {
                format!("%(1j.{}.)", colored_text)
            }
            AppearingCondition::OnError => {
                format!("%(?..{})", colored_text)
            }
            AppearingCondition::OnSuccess => {
                format!("%(?.{}.)", colored_text)
            }
        };
        write!(formatter, "{}", format)
    }
}

/// An abstraction of a part of the prompt, to make it modular.
pub struct PromptComponent {
    structure: String,
}

impl PromptComponent {
    /// Creates an empty instance of `PromptComponent`.
    pub fn new() -> Self {
        Self {
            structure: String::new(),
        }
    }

    /// Creates a instance of `PromptComponent` populated by the value used as
    /// parameter.
    pub fn from<A: Display>(part: A) -> Self {
        let part: String = part.to_string();
        Self { structure: part }
    }

    /// Adds a new element to the `PromptComponent` structure.
    pub fn push<A: Display>(&mut self, part: A) {
        let part: String = part.to_string();
        self.structure.push_str(&part);
    }

    /// Returns the structure of the `PromptComponent`.
    pub fn get_structure(&self) -> String {
        self.structure.clone()
    }
}

/// An abstraction of a prompt.
pub struct Prompt {
    components: Vec<PromptComponent>,
}

impl Prompt {
    /// Creates an empty instance of `Prompt`.
    pub fn new() -> Self {
        Self {
            components: Vec::new(),
        }
    }

    /// Adds a new component to the `Prompt` structure.
    pub fn push(&mut self, component: PromptComponent) {
        self.components.push(component);
    }
}

impl Display for Prompt {
    fn fmt(&self, formatter: &mut Formatter) -> Result {
        let mut format: String = String::new();
        for component in &self.components {
            format.push_str(&component.get_structure());
        }
        write!(formatter, "{}", format)
    }
}
