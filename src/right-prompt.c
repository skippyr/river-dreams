#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define WRITE_ENTRY_TYPE(a_symbol, a_color, a_value) \
    if (a_value) \
    { \
        printf(" %%F{%s}%s%%f%zu", a_color, a_symbol, a_value); \
    }

struct EntryTypes
{
    size_t regulars;
    size_t directories;
    size_t blocks;
    size_t characters;
    size_t sockets;
    size_t fifos;
    size_t symlinks;
};

struct linux_dirent64
{
    ino64_t d_ino;
    off64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[];
};

static void countEntryTypes(struct EntryTypes *types);
static void writeEntryTypes(void);
static void writeJobs(void);

static void countEntryTypes(struct EntryTypes *types)
{
    char buffer[1024];
    struct linux_dirent64 *entry;
    long totalOfEntries;
    long index;
    int directory = open(".", O_RDONLY);
    if (directory < 0)
    {
        return;
    }
    while ((totalOfEntries = syscall(SYS_getdents64, directory, buffer, sizeof(buffer))) > 0)
    {
        for (index = 0; index < totalOfEntries; index += entry->d_reclen)
        {
            entry = (struct linux_dirent64 *)(buffer + index);
            if (*entry->d_name == '.' &&
                (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2])))
            {
                continue;
            }
            if (entry->d_type == DT_REG)
            {
                ++types->regulars;
            }
            else if (entry->d_type == DT_DIR)
            {
                ++types->directories;
            }
            else if (entry->d_type == DT_BLK)
            {
                ++types->blocks;
            }
            else if (entry->d_type == DT_CHR)
            {
                ++types->characters;
            }
            else if (entry->d_type == DT_SOCK)
            {
                ++types->sockets;
            }
            else if (entry->d_type == DT_FIFO)
            {
                ++types->fifos;
            }
            else if (entry->d_type == DT_LNK)
            {
                ++types->symlinks;
            }
        }
        close(directory);
    }
}

static void writeEntryTypes(void)
{
    struct EntryTypes types = {0, 0, 0, 0, 0, 0, 0};
    countEntryTypes(&types);
    WRITE_ENTRY_TYPE(" ", "blue", types.regulars);
    WRITE_ENTRY_TYPE(" ", "yellow", types.directories);
    WRITE_ENTRY_TYPE("󰇖 ", "magenta", types.blocks);
    WRITE_ENTRY_TYPE("󱣴 ", "green", types.characters);
    WRITE_ENTRY_TYPE("󱄙 ", "cyan", types.sockets);
    WRITE_ENTRY_TYPE("󰟦 ", "blue", types.fifos);
    WRITE_ENTRY_TYPE("󰌷 ", "blue", types.symlinks);
}

static void writeJobs(void)
{
    printf("%%(1j. %%F{magenta} %%f%%j.)");
}

int main(void)
{
    writeEntryTypes();
    writeJobs();
    putchar('\n');
    return (0);
}
