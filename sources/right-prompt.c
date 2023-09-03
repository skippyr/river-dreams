#include <sys/stat.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "heap-memory.h"

#define Print_Status__(status, color, string)                                  \
    if (status)                                                                \
    {                                                                          \
        printf(" %%F{%i}%s%%f%ld", color, string, status);                     \
    }

struct Directory_Entries_Status
{
    size_t total_of_hidden,
           total_of_executables,
           total_of_symlinks;
};

static void Get_Directory_Entries_Status(
    struct Directory_Entries_Status *status);
static void Print_Directory_Entries_Status(void);

static void
Get_Directory_Entries_Status(struct Directory_Entries_Status *status)
{
    memset(status, 0, sizeof(struct Directory_Entries_Status));
    DIR *stream = opendir(".");
    if (!stream)
    {
        Errors__Throw_Error("can't open the current directory.");
    }
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            continue;
        }
        char *path = Heap_Memory__Append_Strings("./", entry->d_name);
        struct stat metadata;
        if (lstat(path, &metadata))
        {
            Errors__Throw_Error("can't stat entries in the current directory.");
        }
        Heap_Memory__Deallocate(path);
        if (entry->d_name[0] == '.')
        {
            status->total_of_hidden++;
        }
        if (S_ISLNK(metadata.st_mode))
        {
            status->total_of_symlinks++;
        }
        if (!S_ISDIR(metadata.st_mode) && metadata.st_mode & S_IXUSR)
        {
            status->total_of_executables++;
        }
    }
    closedir(stream);
}

static void
Print_Directory_Entries_Status(void)
{
    struct Directory_Entries_Status status;
    Get_Directory_Entries_Status(&status);
    Print_Status__(status.total_of_hidden, 1, " ");
    Print_Status__(status.total_of_executables, 2, "󱖏 ");
    Print_Status__(status.total_of_symlinks, 4, "󰌷 ");
}

int
main(void)
{
    Print_Directory_Entries_Status();
    printf("%%(1j. %%F{5} %%f %%j.)\n");
    return (EXIT_SUCCESS);
}
