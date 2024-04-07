#define _GNU_SOURCE
#include <dirent.h>

#include <stdio.h>
#include <string.h>

#define WRITE_ENTRY_TYPE_COUNT(a_color, a_symbol, a_value) \
  if (a_value) { \
    if (a_color) { \
      printf(" %%F{%s}%s%%f%zu", a_color, a_symbol, a_value); \
    } else { \
      printf("%s%zu", a_symbol, a_value); \
    } \
  }

struct EntryTypesCount {
  size_t regulars;
  size_t directories;
  size_t blocks;
  size_t characters;
  size_t sockets;
  size_t fifos;
  size_t symlinks;
};

static void countEntryTypes(struct EntryTypesCount *count);
static void writeEntryTypesCountModule(void);
static void writeJobsCountModule(void);
