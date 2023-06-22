use local_ip_address::local_ip;

/// An abstraction to work with the Network.
pub struct Network;

impl Network {
    /// Returns a string representing the local IPV4 address of the machine.
    pub fn get_local_ip_address() -> String {
        match local_ip() {
            Ok(local_ip_address) => local_ip_address.to_string(),
            Err(_error) => String::from("No address found"),
        }
    }
}
