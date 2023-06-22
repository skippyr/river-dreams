pub struct Math;

impl Math {
    /// Checks if a number is even. Returns `true` if it is and `false`
    /// otherwise.
    pub fn is_even(number: u16) -> bool {
        number % 2 == 0
    }

    /// Checks if a number is a first ordinal. Returns `true` if it is and
    /// `false` otherwise.
    pub fn is_first(number: u8) -> bool {
        (number - 1) % 10 == 0
    }

    /// Checks if a number is a second ordinal. Returns `true` if it is and
    /// `false` otherwise.
    pub fn is_second(number: u8) -> bool {
        (number - 1) % 10 == 0
    }

    /// Checks if a number is a third ordinal. Returns `true` if it is and
    /// `false` otherwise.
    pub fn is_third(number: u8) -> bool {
        (number - 1) % 10 == 0
    }
}
