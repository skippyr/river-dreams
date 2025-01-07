//! A module that contains metadata related to the operating system of the computer.

/// Represents the operating system of the computer.
pub struct OperatingSystem;

impl OperatingSystem {
    /// Gets the name of the operating system the theme is running on.
    ///
    /// # Returns
    /// The name of the operating system the theme is running on.
    pub fn name() -> &'static str {
        #[cfg(target_os = "macos")]
        {
            "macOS"
        }
        #[cfg(target_os = "linux")]
        {
            "Linux"
        }
    }
}
