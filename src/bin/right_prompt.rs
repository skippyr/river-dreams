use river_dreams::
{
	file_system::
	{
		get_quantity_of_directory_entry_types_in_current_directory,
		QuantityOfDirectoryEntryTypes
	},
	symbols::Symbol,
	styles::
	{
		colorize,
		Color
	}
};

fn print_quantity_of_directory_entry_types()
{
	let mut quantity_of_directory_entry_types_strings: Vec<String> = Vec::new();
	let quantity_of_directory_entry_types: QuantityOfDirectoryEntryTypes = get_quantity_of_directory_entry_types_in_current_directory();
	if quantity_of_directory_entry_types.executable > 0
	{
		let symbol: Symbol = Symbol
		{
			default_text: String::from(" "),
			fallback_text: String::from("Executable "),
			color: Color::Green
		};
		quantity_of_directory_entry_types_strings.push(format!(
			"{}{}",
			colorize(
				symbol.get_text_for_environment(),
				symbol.color
			),
			quantity_of_directory_entry_types.executable
		));
	}
	if quantity_of_directory_entry_types.hidden > 0
	{
		let symbol: Symbol = Symbol
		{
			default_text: String::from(" "),
			fallback_text: String::from("Hidden "),
			color: Color::Red
		};
		quantity_of_directory_entry_types_strings.push(format!(
			"{}{}",
			colorize(
				symbol.get_text_for_environment(),
				symbol.color
			),
			quantity_of_directory_entry_types.hidden
		));
	}
	if quantity_of_directory_entry_types.symlink > 0
	{
		let symbol: Symbol = Symbol
		{
			default_text: String::from(" "),
			fallback_text: String::from("Symlink "),
			color: Color::Blue
		};
		quantity_of_directory_entry_types_strings.push(format!(
			"{}{}",
			colorize(
				symbol.get_text_for_environment(),
				symbol.color
			),
			quantity_of_directory_entry_types.symlink
		));
	}
	print!(
		"{}",
		quantity_of_directory_entry_types_strings.join(" ")
	);
}

fn main()
{ print_quantity_of_directory_entry_types(); }

