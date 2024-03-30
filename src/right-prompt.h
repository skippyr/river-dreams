#define _GNU_SOURCE
#include <dirent.h>

#include <stdio.h>
#include <string.h>

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
