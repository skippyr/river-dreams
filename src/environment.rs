///! A module to work with environment variables.
use std::env::var;

/// An abstraction to work with environment variables.
pub struct EnvironmentVariables;

impl EnvironmentVariables {
    /// Checks what value the environment variable
    /// `${RIVER_DREAMS_USE_FALLBACK_TEXT}` is set. Returns `true` if it is to
    /// use fallback text and `false` otherwise.
    pub fn is_to_use_fallback_text() -> bool {
        match var("RIVER_DREAMS_USE_FALLBACK_TEXT") {
            Ok(value) => value == "1",
            Err(_error) => false,
        }
    }

    /// Returns the value of the environment variable `${PWD}`. If not set, it
    /// returns the root directory: `/`.
    pub fn get_pwd() -> String {
        match var("PWD") {
            Ok(value) => value,
            Err(_error) => String::from("/"),
        }
    }

    /// Returns the value of the environment variable `${VIRTUAL_ENV}`. If set,
    /// returns a `Some` containing the value, otherwise `None`.
    pub fn get_virtual_env() -> Option<String> {
        match var("VIRTUAL_ENV") {
            Ok(value) => Some(value),
            Err(_error) => None,
        }
    }

    /// Returns the value of the environment variable `${HOME}`. If not set,
    /// it returns an empty string.
    pub fn get_home() -> String {
        match var("HOME") {
            Ok(value) => value,
            Err(_error) => String::new(),
        }
    }
}
