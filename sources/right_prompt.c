#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "common.c"

void
print_background_jobs_quantity(void)
{
	printf(
		" %%(1j.%%F{green}%s%%f%%j.)",
		is_to_use_fallback_text()
		? "Jobs "
		: " "
	);
	return;
}

void
print_directory_entry_types_quantity(void)
{
	DIR *directory_stream = opendir(".");
	struct dirent *directory_entry;
	unsigned short int hidden_entries_quantity = 0;
	unsigned short int executable_entries_quantity = 0;
	unsigned short int symbolic_link_entries_quantity = 0;
	while ((directory_entry = readdir(directory_stream)) != NULL)
	{
		if (
			!strcmp(
				directory_entry->d_name,
				"."
			)
			|| !strcmp(
				directory_entry->d_name,
				".."
			)
		)
		{
			continue;
		}
		struct stat directory_entry_status;
		stat(
			directory_entry->d_name,
			&directory_entry_status
		);
		if (directory_entry_status.st_mode == 33261)
		{
			++executable_entries_quantity;
		}
		if (directory_entry->d_name[0] == '.')
		{
			++hidden_entries_quantity;
		}
		if (directory_entry->d_type == 10)
		{
			++symbolic_link_entries_quantity;
		}
	}
	if (hidden_entries_quantity > 0)
	{
		printf(
			" %%F{red}%s%%f%u",
			is_to_use_fallback_text()
			? "Hidden "
			: " ",
			hidden_entries_quantity
		);
	}
	if (executable_entries_quantity > 0)
	{
		printf(
			" %%F{green}%s%%f%u",
			is_to_use_fallback_text()
			? "Executable "
			: " ",
			executable_entries_quantity
		);
	}
	if (symbolic_link_entries_quantity > 0)
	{
		printf(
			" %%F{blue}%s%%f%u\n",
			is_to_use_fallback_text()
			? "Symlink "
			: " ",
			symbolic_link_entries_quantity
		);
	}
	closedir(directory_stream);
	return;
}

int
main(void)
{
	print_background_jobs_quantity();
	print_directory_entry_types_quantity();
	return (0);
}

