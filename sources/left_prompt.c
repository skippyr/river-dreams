#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <limits.h>
#include <dirent.h>
#include "lib.c"

static void
print_separator(void)
{
	struct winsize terminal_size;
	ioctl(0, TIOCGWINSZ, &terminal_size);

	for (
		unsigned short int iterator = 0;
		iterator != terminal_size.ws_col;
		++iterator
	) {
		if (iterator % 2 == 0) {
			printf(
				"%%F{red}%s%%f",
				choose_symbol_by_environment("▲", "=")
			);
		} else {
			printf(
				"%%F{yellow}%s%%f",
				choose_symbol_by_environment("▼", "-")
			);
		}
	}
}

static void
print_top_left_connector(void)
{
	printf(
		"%%F{red}%s─%%F{yellow}{%%f",
		choose_symbol_by_environment("╭", "┌")
	);
}

static void
print_clock(void)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	if (!strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "0")) {
		if (local_time->tm_hour < 6) { printf("%%F{cyan} "); }
		else if (local_time->tm_hour < 12) { printf("%%F{red}盛"); }
		else if (local_time->tm_hour < 18) { printf("%%F{blue} "); }
		else { printf("%%F{yellow}󰽥 "); }
	} else { printf("%%F{yellow}CLOCK "); }

	printf(
		"%%f%s%dh%s%dm",
		local_time->tm_hour < 10 ? "0" : "",
		local_time->tm_hour,
		local_time->tm_min < 10 ? "0" : "",
		local_time->tm_min
	);
}

static void
print_local_ipv4_address(void)
{
	char host_name[HOST_NAME_MAX + 1];
	gethostname(host_name, sizeof(host_name));

	struct hostent *host_entry = gethostbyname(host_name);

	if (
		host_entry == NULL ||
		host_entry->h_addrtype != AF_INET
	) { return; }

	printf(
		" %%F{red}%s%%f%s",
		choose_symbol_by_environment(" ", "IP "),
		inet_ntoa(*(struct in_addr *) host_entry->h_addr_list[0])
	);
}

static void
print_disk_usage_percentage(void)
{
	struct statvfs sysdisk_status;
	statvfs("/", &sysdisk_status);
	const unsigned long total = sysdisk_status.f_blocks * sysdisk_status.f_bsize;

	printf(
		" %%F{green}%s%%f%u%%%%",
		choose_symbol_by_environment(" ", "DISK "),
		(unsigned short int) (((
			total - sysdisk_status.f_bfree * sysdisk_status.f_bsize) / (float) total
		) * 100)
	);
}

static void
print_python_environment(void)
{
	char *python_environment = getenv("VIRTUAL_ENV");

	if (python_environment != NULL) {
		printf(
			" %%F{red}%s%%f%s",
			choose_symbol_by_environment("󰚐 ", "PYENV "),
			basename(python_environment)
		);
	}
}

static void
print_top_right_connector(void)
{
	printf("%%F{yellow}}%%f\n");
}

static void
print_bottom_left_connector(void)
{
	printf(
		"%%F{red}%s%%f",
		choose_symbol_by_environment("╰", "└")
	);
}

static void
print_shell_status_decorators(void)
{
	printf(
		"%%(?..%%F{yellow}{%%F{red}%s%%?%%F{yellow}}%%f)%%(!.%%F{yellow}{%%F{red}#%%F{yellow}}.)%%(?.%%F{yellow}.%%F{red})%s%%f",
		choose_symbol_by_environment(" ", "X "),
		choose_symbol_by_environment("⤐  ", "> ")
	);
}

static unsigned short int
has_ownership(const char *path)
{
	const unsigned int user_uid = getuid();
	struct stat status;
	stat(path, &status);
	
	if (
		status.st_uid == user_uid ||
		user_uid == 0
	) { return 1; }

	return 0;
}

static void
print_directory_path_abbreviated(void)
{
	char *current_directory_path = getenv("PWD");
	unsigned short int has_ownership_of_current_directory = has_ownership(current_directory_path);
	const char *home_directory_path = getenv("HOME");

	if (strstr(current_directory_path, home_directory_path) != NULL) {
		snprintf(
			current_directory_path,
			strlen(current_directory_path),
			"%s%s",
			"~",
			current_directory_path + strlen(home_directory_path)
		);
	}

	if (!strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "0")) {
		printf("%%F{red}");

		if (!strcmp(current_directory_path, "~")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/Downloads")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/Documents")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/Pictures")) { printf("  "); }
		else if (!strcmp(current_directory_path, "~/Music")) { printf("🎜 "); }
		else if (!strcmp(current_directory_path, "~/Videos")) { printf("󰨜 "); }
		else if (!strcmp(current_directory_path, "~/Public")) { printf("  "); }
		else if (!strcmp(current_directory_path, "~/Templates")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/Desktop")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/.local")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/.config")) { printf(" "); }
		else if (!strcmp(current_directory_path, "~/.cache")) { printf(" "); }
	}

	unsigned short int path_slice_last_index = 0;
	for (
		unsigned short int iterator = 0;
		!(iterator == strlen(current_directory_path));
		++iterator
	) {
		if (
			current_directory_path[iterator] == '/' &&
			iterator != 0
		) { ++path_slice_last_index; }
	}

	printf("%%F{green}");

	unsigned short int path_slice_index = 0;
	for (
		unsigned short int iterator = 0;
		!(iterator == strlen(current_directory_path));
		++iterator
	) {
		if (
			current_directory_path[iterator] == '/' &&
			iterator != 0
		) { ++path_slice_index; }
		if (
			path_slice_index == path_slice_last_index ||
			current_directory_path[iterator] == '/' ||
			current_directory_path[iterator] == '.' ||
			current_directory_path[iterator] == '~' ||
			current_directory_path[iterator - 1] == '/' ||
			(
				current_directory_path[iterator - 2] == '/' &&
				current_directory_path[iterator - 1] == '.'
			)
		) { printf("%c", current_directory_path[iterator]); }
	}

	printf(
		"%%F{red}%s%%f",
		has_ownership_of_current_directory
		? ""
		: choose_symbol_by_environment(" ", " LOCKED")
	);
}

static unsigned short int
get_dot_git_parent_directory_path(const char *relative_path, char directory_path[])
{
	DIR *directory_stream = opendir(relative_path);
	struct dirent *directory_entry;
	realpath(relative_path, directory_path); 

	while ((directory_entry = readdir(directory_stream)) != NULL) {
		if (
			directory_entry->d_type == 4 &&
			!strcmp(directory_entry->d_name, ".git")
		) { return 0; }
	}

	closedir(directory_stream);

	if (!strcmp(directory_path, "/")) { return 1; }

	get_dot_git_parent_directory_path(dirname(directory_path), directory_path);
}

static void
print_git_branch(void)
{
	char head_file_path[PATH_MAX];
	if (get_dot_git_parent_directory_path(".", head_file_path) != 0) { return; }
	strcat(
		head_file_path,
		"/.git/HEAD"
	);

	FILE *file_stream = fopen(head_file_path, "r");
	char buffer[2];

	printf(" %%F{red}");

	unsigned short int slashes_passed = 0;

	while (fgets(buffer, sizeof(buffer), file_stream) != NULL) {
		if (!strcmp(buffer, "/")) {
			++slashes_passed;
		} else if (
			slashes_passed == 2 &&
			strcmp(buffer, "\n")
		) {
			printf("%s", buffer);
		}
	}

	fclose(file_stream);

	printf("%%f");
}

static void
print_cursor_decorator(void)
{
	printf(" %%F{yellow}%s%%f", choose_symbol_by_environment("✗ ", "X "));
}

int
main(void)
{
	print_separator();
	print_top_left_connector();
	print_clock();
	print_local_ipv4_address();
	print_disk_usage_percentage();
	print_python_environment();
	print_top_right_connector();
	print_bottom_left_connector();
	print_shell_status_decorators();
	print_directory_path_abbreviated();
	print_git_branch();
	print_cursor_decorator();

	return 0;
}

