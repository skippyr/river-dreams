use local_ip_address::local_ip;

pub struct Network;

impl Network {
	pub fn get_local_ip_address() -> String {
		match local_ip() {
			Ok(local_ip_address) => {
				local_ip_address.to_string()
			}
			Err(_error) => {
				String::from("No address found")
			}
		}
	}
}

