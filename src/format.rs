//! A module that contains functionalities related to formatting.

/// Calculates the total number of digits in a number.
///
/// # Parameters
/// - `number`: the number to be checked.
///
/// # Returns
/// The total number of digits in the number.
pub fn calculate_total_digits(mut number: u16) -> u16 {
    let mut total_digits = if number == 0 { 1 } else { 0 };
    loop {
        if number == 0 {
            break;
        }
        total_digits += 1;
        number /= 10;
    }
    total_digits
}
