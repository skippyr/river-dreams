use crossterm::terminal::size;

/// An abstraction to work terminal emulator's properties.
pub struct TerminalEmulator;

impl TerminalEmulator {
    /// Returns the number of columns of the terminal emulator. If none of the
    /// streams consulted is a tty, it returns `0`.
    pub fn get_columns() -> u16 {
        match size() {
            Ok(size) => size.0,
            Err(_error) => 0,
        }
    }
}
