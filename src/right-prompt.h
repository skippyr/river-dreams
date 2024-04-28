#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

struct EntriesCount
{
    int totalFiles;
    int totalDirectories;
    int totalSymlinks;
    int totalBlocks;
    int totalCharacters;
    int totalFifos;
    int totalSockets;
    int totalHiddenEntries;
    int totalTemporaryEntries;
};

struct linux_dirent64
{
    ino64_t d_ino;
    off64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

static void countEntries(struct EntriesCount *count);
static void writeEntryCount(const char *symbol, const char *color, int value);
static void writeEntryModule(void);
static void writeJobModule(void);
