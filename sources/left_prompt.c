#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include "lib.c"

int
has_ownership(char *path)
{
	unsigned int user_uid = getuid();
	struct stat status;
	stat(path, &status);
	if (status.st_uid == user_uid || user_uid == 0) {
		return 1;
	}
	return 0;
}

void
print_separator(void)
{
	struct winsize terminal_size;
	ioctl(0, TIOCGWINSZ, &terminal_size);

	for (int i = 1; !(i > terminal_size.ws_col); ++i) {
		if (i % 2 == 0) {
			printf("%%F{red}%s%%f", choose_symbol_by_environment("▲", "/"));
		} else {
			printf(
				"%%F{%s}%s%%f",
				choose_color_by_environment("yellow", "blue"),
				choose_symbol_by_environment("▼", "\\")
			);
		}
	}
}

void
print_top_connector_left(void)
{
	printf("%%F{red}┌─%%F{yellow}[%%f");
}

void
print_clock(void)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	if (!strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "0")) {
		if (local_time->tm_hour < 6) {
			printf("%%F{cyan} ");
		} else if (local_time->tm_hour < 12) {
			printf("%%F{red}盛 ");
		} else if (local_time->tm_hour < 18) {
			printf("%%F{blue} ");
		} else {
			printf("%%F{yellow}󰽥 ");
		}
	} else {
		printf("%%F{yellow}CLOCK ");
	}

	printf(
		"%%f%s%dh%s%dm",
		local_time->tm_hour < 10 ? "0" : "",
		local_time->tm_hour,
		local_time->tm_min < 10 ? "0" : "",
		local_time->tm_min
	);
}

void
print_disk_usage_percentage(void)
{
	struct statvfs sysdisk_status;
	statvfs("/", &sysdisk_status);
	unsigned long total = sysdisk_status.f_blocks * sysdisk_status.f_bsize;

	printf(
		" %%F{green}%s%%f%u%%%%",
		choose_symbol_by_environment(" ", "DISK "),
		(unsigned int) (((
			total - sysdisk_status.f_bfree * sysdisk_status.f_bsize) / (float) total
		) * 100)
	);
}

void
print_pyenv(void)
{
	char *pyenv = getenv("VIRTUAL_ENV");

	if (pyenv != NULL) {
		printf(
			" %%F{red}%s%%f%s",
			choose_symbol_by_environment("󰚐 ", "PYENV "),
			basename(pyenv)
		);
	}
}

void
print_top_connector_right(void)
{
	printf("%%F{yellow}]%%f\n");
}

void
print_bottom_connector(void)
{
	printf("%%F{red}└%%f");
}

void
print_arrow(void)
{
	printf(
		"%%(?..%%F{yellow}[%%F{red}%s%%B%%?%%b%%F{yellow}]%%f)%%(!.%%F{yellow}[%%F{red}#%%F{yellow}].)%%(?.%%F{yellow}.%%F{red})%s%%f",
		choose_symbol_by_environment(" ", "X "),
		choose_symbol_by_environment("⤐  ", "> ")
  );
}

void
print_directory(void)
{
	char *current_directory_path = getenv("PWD");
	int has_ownership_of_current_directory = has_ownership(current_directory_path);
	char *home_directory_path = getenv("HOME");

	if (strstr(current_directory_path, home_directory_path) != NULL) {
		snprintf(
			current_directory_path,
			strlen(current_directory_path),
			"%s%s",
			"~",
			current_directory_path + strlen(home_directory_path)
		);
	}

	int path_slice_last_index = 0;
	for (int i = 0; i != strlen(current_directory_path); ++i) {
		if (current_directory_path[i] == '/' && i != 0) {
			++path_slice_last_index;
		}
	}

	printf("%%F{green}");

	int path_slice_index = 0;
	for (int i = 0; i != strlen(current_directory_path); ++i) {
		if (current_directory_path[i] == '/' && i != 0) {
			++path_slice_index;
		}
		if (
			path_slice_index == path_slice_last_index ||
			current_directory_path[i] == '/' ||
			current_directory_path[i] == '.' ||
			current_directory_path[i] == '~' ||
			current_directory_path[i - 1] == '/' ||
			(
				current_directory_path[i - 2] == '/' &&
				current_directory_path[i - 1] == '.'
			)
		) {
			printf("%c", current_directory_path[i]);
		}
	}

	printf(
		"%%F{red}%s%%f",
		has_ownership_of_current_directory
		? ""
		: choose_symbol_by_environment(" ", " LOCKED")
	);
}

int
main(void)
{
	print_separator();
	print_top_connector_left();
	print_clock();
	print_disk_usage_percentage();
	print_pyenv();
	print_top_connector_right();
	print_bottom_connector();
	print_arrow();
	print_directory();
	return 0;
}

