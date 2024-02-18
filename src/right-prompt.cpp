#include <dirent.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdio>

#define WRITE_ENTRY_TYPE(color_m, symbol_m, value_m) \
    if (value_m) \
    { \
        std::printf(" %%F{%s}%s%%f%ld", color_m, symbol_m, value_m); \
    }

struct EntryTypes
{
    std::size_t regulars = 0;
    std::size_t directories = 0;
    std::size_t blocks = 0;
    std::size_t characters = 0;
    std::size_t sockets = 0;
    std::size_t fifos = 0;
    std::size_t symlinks = 0;
};

struct linux_dirent64
{
    ino64_t d_ino;
    off64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[];
};

static void GetEntryTypes(struct EntryTypes& types);

static void GetEntryTypes(struct EntryTypes& types)
{
    char buffer[1024];
    int directory = open(".", O_RDONLY);
    long totalOfEntries;
    long index;
    struct linux_dirent64* entry;
    if (directory < 0)
    {
        return;
    }
    while ((totalOfEntries = syscall(SYS_getdents64, directory, buffer, sizeof(buffer))) > 0)
    {
        for (index = 0; index < totalOfEntries; index += entry->d_reclen)
        {
            entry = (struct linux_dirent64*)(buffer + index);
            if (*entry->d_name == '.' &&
                (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2])))
            {
                continue;
            }
            switch (entry->d_type)
            {
            case DT_REG:
                types.regulars++;
                break;
            case DT_DIR:
                types.directories++;
                break;
            case DT_BLK:
                types.blocks++;
                break;
            case DT_CHR:
                types.characters++;
                break;
            case DT_SOCK:
                types.sockets++;
                break;
            case DT_FIFO:
                types.fifos++;
                break;
            case DT_LNK:
                types.symlinks++;
                break;
            }
        }
    }
    close(directory);
}

int main(void)
{
    struct EntryTypes types = EntryTypes();
    GetEntryTypes(types);
    WRITE_ENTRY_TYPE("blue", " ", types.regulars);
    WRITE_ENTRY_TYPE("yellow", " ", types.directories);
    WRITE_ENTRY_TYPE("magenta", "󰇖 ", types.blocks);
    WRITE_ENTRY_TYPE("green", "󱣴 ", types.characters);
    WRITE_ENTRY_TYPE("blue", "󰟦 ", types.fifos);
    WRITE_ENTRY_TYPE("cyan", "󱄙 ", types.sockets);
    WRITE_ENTRY_TYPE("blue", "󰌷 ", types.symlinks);
    std::printf("%%(1j. %%F{magenta} %%f%%j.)\n");
    return (0);
}
