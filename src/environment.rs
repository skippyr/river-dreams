use std::env::var;

pub struct EnvironmentVariables;

impl EnvironmentVariables {
    pub fn is_to_use_fallback_text() -> bool {
        match var("RIVER_DREAMS_USE_FALLBACK_TEXT") {
            Ok(value) => value == "1",
            Err(_error) => false,
        }
    }

    pub fn get_pwd() -> String {
        match var("PWD") {
            Ok(value) => value,
            Err(_error) => String::from("/"),
        }
    }

    pub fn get_virtual_env() -> Option<String> {
        match var("VIRTUAL_ENV") {
            Ok(value) => Some(value),
            Err(_error) => None,
        }
    }

    pub fn get_home() -> String {
        match var("HOME") {
            Ok(value) => value,
            Err(_error) => String::new(),
        }
    }
}
