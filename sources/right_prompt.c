#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "lib.c"

static void
print_background_jobs_quantity(void)
{
	printf(
		" %%(1j.%%F{green}%s%%f%%j.)",
		is_to_use_fallback_text() ? "JOBS " : " "
	);
}

static void
print_directory_entry_types_quantity(void)
{
	DIR *directory_stream = opendir(".");
	struct dirent *entry;

	unsigned short int hidden_entries_quantity = 0;
	unsigned short int executable_entries_quantity = 0;
	unsigned short int symbolic_link_entries_quantity = 0;

	while ((entry = readdir(directory_stream)) != NULL) {
		if (
			!strcmp(entry->d_name, ".") ||
			!strcmp(entry->d_name, "..")
		) { continue; }

		struct stat entry_status;
		stat(entry->d_name, &entry_status);

		if (entry_status.st_mode == 33261) { ++executable_entries_quantity; }
		if (entry->d_name[0] == '.') { ++hidden_entries_quantity; }
		if (entry->d_type == 10) { ++symbolic_link_entries_quantity; }
	}

	if (hidden_entries_quantity > 0) {
		printf(
			" %%F{red}%s%%f%u",
			is_to_use_fallback_text() ? "HIDDEN " : " ",
			hidden_entries_quantity
		);
	}
	if (executable_entries_quantity > 0) {
		printf(
			" %%F{green}%s%%f%u",
			is_to_use_fallback_text() ? "EXECUTABLE " : " ",
			executable_entries_quantity
		);
	}
	if (symbolic_link_entries_quantity > 0) {
		printf(
			" %%F{blue}%s%%f%u\n",
			is_to_use_fallback_text() ? "SYMLINK " : " ",
			symbolic_link_entries_quantity
		);
	}

	closedir(directory_stream);
}

int
main(void)
{
	print_background_jobs_quantity();
	print_directory_entry_types_quantity();

	return 0;
}

