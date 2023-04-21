#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "common.c"

#define EXECUTABLE_MODE 33261
#define SYMBOLIC_LINK_TYPE 10

void print_quantity_of_background_jobs(void)
{
	printf(
		" %%(1j.%%F{green}%s%%f%%j.)",
		is_to_use_fallback_text() ?
		"Jobs " :
		" "
	);
	return;
}

void print_quantity_of_directory_entry_types(void)
{
	DIR *directory_stream = opendir(".");
	if (directory_stream == NULL)
	{
		return;
	}
	struct dirent *directory_entry;
	unsigned short int
		quantity_of_hidden_entries = 0,
		quantity_of_executable_entries = 0,
		quantity_of_symbolic_link_entries = 0;
	while ((directory_entry = readdir(directory_stream)) != NULL)
	{
		if (
			!strcmp(
				directory_entry->d_name,
				"."
			) ||
			!strcmp(
				directory_entry->d_name,
				".."
			)
		)
		{
			continue;
		}
		struct stat directory_entry_status;
		if (stat(
			directory_entry->d_name,
			&directory_entry_status
		) != 0)
		{
			continue;
		}
		if (directory_entry_status.st_mode == EXECUTABLE_MODE)
		{
			++quantity_of_executable_entries;
		}
		if (directory_entry->d_name[0] == '.')
		{
			++quantity_of_hidden_entries;
		}
		if (directory_entry->d_type == SYMBOLIC_LINK_TYPE)
		{
			++quantity_of_symbolic_link_entries;
		}
	}
	if (quantity_of_hidden_entries > 0)
	{
		printf(
			" %%F{red}%s%%f%u",
			is_to_use_fallback_text() ?
			"Hidden " :
			" ",
			quantity_of_hidden_entries
		);
	}
	if (quantity_of_executable_entries > 0)
	{
		printf(
			" %%F{green}%s%%f%u",
			is_to_use_fallback_text() ?
			"Executable " :
			" ",
			quantity_of_executable_entries
		);
	}
	if (quantity_of_symbolic_link_entries > 0)
	{
		printf(
			" %%F{blue}%s%%f%u\n",
			is_to_use_fallback_text() ?
			"Symlink " :
			" ",
			quantity_of_symbolic_link_entries
		);
	}
	closedir(directory_stream);
	return;
}

int main(void)
{
	print_quantity_of_background_jobs();
	print_quantity_of_directory_entry_types();
	return (0);
}

