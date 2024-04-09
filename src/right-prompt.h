#define _GNU_SOURCE
#include <dirent.h>

#include <stdio.h>
#include <string.h>

#define WRITE_ENTRY_TYPE_COUNT(color_a, symbol_a, value_a) \
	if (value_a) \
	{ \
		if (color_a) \
		{ \
			printf(" %%F{%s}%s%%f%zu", color_a, symbol_a, value_a); \
		} \
		else \
		{ \
			printf("%s%zu", symbol_a, value_a); \
		} \
	}

struct EntryTypesCount
{
	size_t regulars;
	size_t directories;
	size_t blocks;
	size_t characters;
	size_t sockets;
	size_t fifos;
	size_t symlinks;
};

static void countEntryTypes(struct EntryTypesCount* count);
static void writeEntryTypesCountModule(void);
static void writeJobsCountModule(void);
