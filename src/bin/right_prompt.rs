use river_dreams::file_system::
{
	get_quantity_of_directory_entry_types_in_current_directory,
	// QuantityOfDirectoryEntryTypes
};

fn main()
{
	eprintln!(
		"{:?}",
		get_quantity_of_directory_entry_types_in_current_directory()
	);
}

