#include <arpa/inet.h>
#include <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>
#include "common.c"

#define EPOCH_YEAR 1900
#define ROOT_UID 0
#define DIRECTORY_TYPE 4

void print_separator(void)
{
	struct winsize terminal_size;
	if (ioctl(
		0,
		TIOCGWINSZ,
		&terminal_size
	) != 0)
	{
		return;
	}
	for (
		unsigned short int iterator = 0;
		iterator != terminal_size.ws_col;
		++iterator
	)
	{
		if (iterator % 2 == 0)
		{
			printf(
				"%%F{red}%s%%f",
				is_to_use_fallback_text() ?
				"=" :
				""
			);
		}
		else
		{
			printf(
				"%s",
				is_to_use_fallback_text() ?
				"-" :
				""
			);
		}
	}
	return;
}

void print_top_left_connector(void)
{
	printf(
		"%%F{red}%s─%%F{yellow}{%%f",
		is_to_use_fallback_text() ?
		"┌" :
		"╭"
	);
	return;
}

void print_time(void)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);
	if (local_time == NULL)
	{
		return;
	}
	printf(" %%F{red}¦ ");
	printf(
		"%s",
		is_to_use_fallback_text() ?
		"Calendar " :
		" "
	);
	printf("(%%f");
	switch (local_time->tm_wday)
	{
		case 0:
			printf("Mon");
			break;
		case 1:
			printf("Sun");
			break;
		case 2:
			printf("Tue");
			break;
		case 3:
			printf("Wed");
			break;
		case 4:
			printf("Thu");
			break;
		case 5:
			printf("Fri");
			break;
		case 6:
			printf("Sat");
			break;
	}
	printf(
		"%%F{red})%%f %d",
		local_time->tm_mday
	);
	if ((local_time->tm_mday - 1) % 10 == 0)
	{
		printf("st");
	}
	else if ((local_time->tm_mday - 2) % 10 == 0)
	{
		printf("nd");
	}
	else if ((local_time->tm_mday - 3) % 10 == 0)
	{
		printf("rd");
	}
	else
	{
		printf("th");
	}
	printf(" ");
	switch (local_time->tm_mon)
	{
		case 0:
			printf("Jan");
			break;
		case 1:
			printf("Feb");
			break;
		case 2:
			printf("Mar");
			break;
		case 3:
			printf("Apr");
			break;
		case 4:
			printf("May");
			break;
		case 5:
			printf("Jun");
			break;
		case 6:
			printf("Jul");
			break;
		case 7:
			printf("Aug");
			break;
		case 8:
			printf("Sep");
			break;
		case 9:
			printf("Oct");
			break;
		case 10:
			printf("Nov");
			break;
		case 11:
			printf("Dec");
			break;
	}
	printf(
		", %d %%F{red}¦ ",
		EPOCH_YEAR + local_time->tm_year
	);
	if (is_to_use_fallback_text())
	{
		printf("%%F{yellow}Clock ");
	}
	else
	{
		if (local_time->tm_hour < 6)
		{
			printf("%%F{cyan} ");
		}
		else if (local_time->tm_hour < 12)
		{
			printf("%%F{red}盛");
		}
		else if (local_time->tm_hour < 18)
		{
			printf("%%F{blue} ");
		}
		else
		{
			printf("%%F{yellow}󰽥 ");
		}
	}
	printf(
		"%%f%s%dh%s%dm",
		local_time->tm_hour < 10 ?
		"0" :
		"",
		local_time->tm_hour,
		local_time->tm_min < 10 ?
		"0" :
		"",
		local_time->tm_min
	);
	return;
}

void print_local_ipv4_address(void)
{
	char host_name[HOST_NAME_MAX];
	if (gethostname(
		host_name,
		sizeof(host_name)
	) != 0)
	{
		return;
	}
	struct hostent *host_entry = gethostbyname(host_name);
	if (
		host_entry == NULL ||
		host_entry->h_addrtype != AF_INET
	)
	{
		return;
	}
	printf(
		"%%F{red}%s%%f%%m%%F{red}@%%f%s",
		is_to_use_fallback_text() ?
		"IP " :
		" ",
		inet_ntoa(*(struct in_addr *) host_entry->h_addr_list[0])
	);
	return;
}

void print_disk_usage_percentage(void)
{
	struct statvfs sysdisk_status;
	if (statvfs(
		"/",
		&sysdisk_status
	) != 0)
	{
		return;
	}
	const unsigned long total = sysdisk_status.f_blocks * sysdisk_status.f_bsize;
	printf(
		" %%F{red}¦ %%F{yellow}%s%%f%u%%%%",
		is_to_use_fallback_text() ?
		"Disk " :
		" ",
		(unsigned short int) (((total - sysdisk_status.f_bfree * sysdisk_status.f_bsize) / (float) total) * 100)
	);
	return;
}

void print_python_environment(void)
{
	char *python_environment = getenv("VIRTUAL_ENV");
	if (python_environment == NULL)
	{
		return;
	}
	printf(
		"using %%F{magenta}%s%%f ",
		basename(python_environment)
	);
	return;
}

void print_top_right_connector(void)
{
	printf("%%F{yellow}}%%f\n");
	return;
}

void print_bottom_left_connector(void)
{
	printf(
		"%%F{red}%s%%f",
		is_to_use_fallback_text() ?
		"└" :
		"╰"
	);
	return;
}

void print_shell_status_decorators(void)
{
	printf(
		"%%(?..%%F{yellow}{%%F{red}%s%%?%%F{yellow}}%%f)%%(?.%%F{yellow}.%%F{red})%s%%f",
		is_to_use_fallback_text() ?
		"X " :
		" ",
		is_to_use_fallback_text() ?
		"> " :
		"⤐  "
	);
	return;
}

unsigned short int has_ownership(const char *path)
{
	uid_t user_uid = getuid();
	struct stat status;
	return (
		stat(
			path,
			&status
		) == 0 ?
		status.st_uid == user_uid ||
		user_uid == ROOT_UID :
		0
	);
}

void print_user(void)
{
	printf("%%F{cyan}%%n%%f ");
	return;
}

void print_current_directory_path_abbreviated(void)
{
	const char *current_directory_path = getenv("PWD");
	const char *home_directory_path = getenv("HOME");
	if (
		current_directory_path == NULL ||
		home_directory_path == NULL
	)
	{
		return;
	}
	char current_directory_path_with_aliases[strlen(current_directory_path) + 1];
	if (strstr(
		current_directory_path,
		home_directory_path
	) == NULL)
	{
		strcat(
			current_directory_path_with_aliases,
			current_directory_path
		);
	}
	else
	{
		snprintf(
			current_directory_path_with_aliases,
			sizeof(current_directory_path_with_aliases),
			"%s%s",
			"~",
			current_directory_path + strlen(home_directory_path)
		);
	}
	printf("at ");
	if (!is_to_use_fallback_text())
	{
		printf("%%F{red}");
		if (!strcmp(
			current_directory_path_with_aliases,
			"~"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Downloads"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Documents"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Pictures"
		))
		{
			printf("  ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Music"
		))
		{
			printf("🎜  ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Videos"
		))
		{
			printf("󰨜 ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Public"
		))
		{
			printf("  ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Templates"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/Desktop"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/.local"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/.config"
		))
		{
			printf(" ");
		}
		else if (!strcmp(
			current_directory_path_with_aliases,
			"~/.cache"
		))
		{
			printf(" ");
		}
	}
	unsigned short int path_slice_last_index = 0;
	for (
		unsigned short int iterator = 0;
		iterator != strlen(current_directory_path_with_aliases);
		++iterator
	)
	{
		if (
			current_directory_path_with_aliases[iterator] == '/' &&
			iterator != 0
		)
		{
			++path_slice_last_index;
		}
	}
	printf("%%F{green}");
	unsigned short int path_slice_index = 0;
	for (
		unsigned short int iterator = 0;
		!(iterator == strlen(current_directory_path_with_aliases));
		++iterator
	)
	{
		if (
			current_directory_path_with_aliases[iterator] == '/' &&
			iterator != 0
		)
		{
			++path_slice_index;
		}
		if (
			path_slice_index == path_slice_last_index ||
			current_directory_path_with_aliases[iterator] == '/' ||
			current_directory_path_with_aliases[iterator] == '~' ||
			current_directory_path_with_aliases[iterator - 1] == '/' ||
			(
				current_directory_path_with_aliases[iterator - 2] == '/' &&
				current_directory_path_with_aliases[iterator - 1] == '.'
			)
		)
		{
			printf(
				"%c",
				current_directory_path_with_aliases[iterator]
			);
		}
	}
	printf(
		"%%F{red}%s%%f",
		has_ownership(current_directory_path) ?
		"" :
		is_to_use_fallback_text() ?
		" #" :
		" "
	);
	return;
}

unsigned short int get_dot_git_parent_directory_path(
	const char *relative_path,
	char directory_path[]
)
{
	DIR *directory_stream = opendir(relative_path);
	if (directory_stream == NULL)
	{
		return (1);
	}
	struct dirent *directory_entry;
	if (realpath(
		relative_path,
		directory_path
	) == NULL)
	{
		return (1);
	}
	while ((directory_entry = readdir(directory_stream)) != NULL)
	{
		if (
			directory_entry->d_type == DIRECTORY_TYPE &&
			!strcmp(
				directory_entry->d_name,
				".git"
			)
		)
		{
			return (0);
		}
	}
	closedir(directory_stream);
	return (
		!strcmp(
			directory_path,
			"/"
		) ?
		1 :
		get_dot_git_parent_directory_path(
			dirname(directory_path),
			directory_path
		)
	);
}

void print_git_branch(void)
{
	char head_file_path[PATH_MAX];
	if (get_dot_git_parent_directory_path(
		".",
		head_file_path
	) != 0)
	{
		return;
	}
	strcat(
		head_file_path,
		"/.git/HEAD"
	);
	FILE *file_stream = fopen(
		head_file_path,
		"r"
	);
	if (file_stream == NULL)
	{
		return;
	}
	char buffer[2];
	printf(" on %%F{red}");
	unsigned short int slashes_passed = 0;
	while (fgets(
		buffer,
		sizeof(buffer),
		file_stream
	) != NULL)
	{
		if (!strcmp(
			buffer,
			"/"
		))
		{
			++slashes_passed;
		}
		else if (
			slashes_passed == 2 &&
			strcmp(
				buffer,
				"\n"
			)
		)
		{
			printf("%s", buffer);
		}
	}
	fclose(file_stream);
	printf("%%f");
	return;
}

void print_cursor_decorator(void)
{
	printf(
		" %%F{yellow}%s%%f",
		is_to_use_fallback_text() ?
		"X " :
		"🗶 "
	);
	return;
}

int main(void)
{
	print_separator();
	print_top_left_connector();
	print_local_ipv4_address();
	print_disk_usage_percentage();
	print_time();
	print_top_right_connector();
	print_bottom_left_connector();
	print_shell_status_decorators();
	print_user();
	print_python_environment();
	print_current_directory_path_abbreviated();
	print_git_branch();
	print_cursor_decorator();
	return (0);
}

