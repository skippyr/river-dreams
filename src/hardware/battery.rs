//! A module that contains metadata related to the battery of the computer.

use crate::error::Error;
use battery::{units::ratio::ratio, Manager as BatteryManager, State as BatteryState};

/// Represents the battery of the computer.
pub struct Battery;

/// Contains predefined statuses for quickly describing the usage of a battery.
#[derive(Clone, Copy)]
pub enum BatteryChargeStatus {
    /// Indicates the battery charge is critical and requires immediate charging.
    Critical,
    /// Indicates the battery charge is low and requires charging.
    Low,
    /// Indicates the battery charge is moderate and may require intervention in the future.
    Moderate,
    /// Indicates the battery charge is high.
    High,
}

impl TryFrom<u8> for BatteryChargeStatus {
    type Error = Error;

    /// Gets the charge status that corresponds to the percentage remaining of a battery.
    ///
    /// # Parameters
    /// - `percentage`: the percentage remaining of a battery.
    ///
    /// # Returns
    /// The charge status that corresponds to the percentage remaining of the battery or an error.
    ///
    /// # Errors
    /// An error happens if the percentage provided is not within range from 0 to 100.
    fn try_from(percentage: u8) -> Result<Self, Self::Error> {
        match percentage {
            0..5 => Ok(Self::Critical),
            5..30 => Ok(Self::Low),
            30..60 => Ok(Self::Moderate),
            60..100 => Ok(Self::High),
            _ => Err(Error::new(format!(
                r#"can not get battery charge status from invalid percentage {percentage}."#
            ))),
        }
    }
}

/// Represents the information about the charge of a battery.
pub struct BatteryCharge {
    /// The charge percentage of the battery, a value within range from 0 to 100.
    percentage: u8,
    /// A boolean that states the battery is charging.
    is_charging: bool,
    /// The status that represents the charge of the battery.
    status: BatteryChargeStatus,
}

impl BatteryCharge {
    /// Gets the charge percentage of the battery.
    ///
    /// # Returns
    /// The charge percentage of the battery.
    pub fn percentage(&self) -> u8 {
        self.percentage
    }

    /// Checks whether the battery is charging.
    ///
    /// # Returns
    /// A boolean that states the battery is charging.
    pub fn is_charging(&self) -> bool {
        self.is_charging
    }

    /// Gets the status that represents the charge of the battery.
    ///
    /// # Returns
    /// The status that represents the charge of the battery.
    pub fn status(&self) -> BatteryChargeStatus {
        self.status
    }
}

impl Battery {
    /// Gets the information about the charge of the battery of the computer.
    ///
    /// # Returns
    /// The information about the charge of the battery of the computer, nothing, or an error.
    ///
    /// Nothing will be returned if no battery is found.
    ///
    /// # Errors
    /// An error happens if the information about the energy supplies of the computer can not be retrieved.
    pub fn charge() -> Result<Option<BatteryCharge>, Error> {
        const SUPPLIES_INFO_ERROR: &str =
            "can not retrieve information about the energy supplies of the computer.";
        for battery in BatteryManager::new()
            .map_err(|_| Error::new(SUPPLIES_INFO_ERROR))?
            .batteries()
            .map_err(|_| Error::new(SUPPLIES_INFO_ERROR))?
        {
            let battery: battery::Battery = match battery {
                Ok(battery) => battery,
                Err(_) => continue,
            };
            let percentage: u8 = (battery.state_of_charge().get::<ratio>() * 100.0).round() as u8;
            return Ok(Some(BatteryCharge {
                percentage,
                status: BatteryChargeStatus::try_from(percentage).unwrap(),
                is_charging: match battery.state() {
                    BatteryState::Charging | BatteryState::Full | BatteryState::Unknown => true,
                    _ => false,
                },
            }));
        }
        Ok(None)
    }
}
