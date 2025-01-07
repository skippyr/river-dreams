//! A module that contains metadata related to the CPU of the computer.

/// Represents the CPU of the computer.
pub struct Cpu;

impl Cpu {
    /// Gets the architecture name of the CPU of the computer.
    ///
    /// # Returns
    /// The architecture name of the CPU of the computer.
    pub fn architecture_name() -> &'static str {
        #[cfg(target_arch = "x86_64")]
        {
            "x86_64"
        }
        #[cfg(target_arch = "aarch64")]
        {
            "ARM64"
        }
    }
}
