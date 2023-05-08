pub fn print_error(
	description: &str,
	suggestion: &str,
	exit_code: u32
)
{
	eprintln!("Opsie!");
	eprintln!("\tprogram: river_dreams.");
	eprintln!(
		"\tdescription: {}",
		description
	);
	eprintln!(
		"\tsuggestion: {}",
		suggestion
	);
	eprintln!(
		"\texit code: {}",
		exit_code
	);
}

