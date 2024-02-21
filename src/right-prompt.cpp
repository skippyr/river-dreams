#include <dirent.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdio>

#define WRITE_ENTRY_TYPE(color_a, symbol_a, value_a) \
    if (value_a) { \
        std::printf(" %%F{%s}%s%%f%ld", color_a, symbol_a, value_a); \
    }

struct EntryTypes
{
    std::size_t regulars_f;
    std::size_t directories_f;
    std::size_t blocks_f;
    std::size_t characters_f;
    std::size_t sockets_f;
    std::size_t fifos_f;
    std::size_t symlinks_f;
};

struct linux_dirent64
{
    ino64_t d_ino;
    off64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[];
};

static struct EntryTypes count_entry_types();
static void write_total_of_entry_types();
static void write_total_of_jobs();

static struct EntryTypes count_entry_types()
{
    char buffer[1024];
    int directory = open(".", O_RDONLY);
    long total_of_entries;
    long entry_index;
    struct linux_dirent64* entry;
    struct EntryTypes types = {0, 0, 0, 0, 0, 0, 0};
    if (directory < 0) {
        return (types);
    }
    while ((total_of_entries = syscall(SYS_getdents64, directory, buffer, sizeof(buffer))) > 0) {
        for (entry_index = 0; entry_index < total_of_entries; entry_index += entry->d_reclen) {
            entry = reinterpret_cast<struct linux_dirent64*>(buffer + entry_index);
            if (*entry->d_name == '.' &&
                (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2]))) {
                continue;
            }
            if (entry->d_type == DT_REG) {
                types.regulars_f++;
            } else if (entry->d_type == DT_DIR) {
                types.directories_f++;
            } else if (entry->d_type == DT_BLK) {
                types.blocks_f++;
            } else if (entry->d_type == DT_CHR) {
                types.characters_f++;
            } else if (entry->d_type == DT_SOCK) {
                types.sockets_f++;
            } else if (entry->d_type == DT_FIFO) {
                types.fifos_f++;
            } else if (entry->d_type == DT_LNK) {
                types.symlinks_f++;
            }
        }
    }
    close(directory);
    return (types);
}

static void write_total_of_entry_types()
{
    struct EntryTypes types = count_entry_types();
    WRITE_ENTRY_TYPE("blue", " ", types.regulars_f);
    WRITE_ENTRY_TYPE("yellow", " ", types.directories_f);
    WRITE_ENTRY_TYPE("magenta", "󰇖 ", types.blocks_f);
    WRITE_ENTRY_TYPE("green", "󱣴 ", types.characters_f);
    WRITE_ENTRY_TYPE("cyan", "󱄙 ", types.sockets_f);
    WRITE_ENTRY_TYPE("blue", "󰟦 ", types.fifos_f);
    WRITE_ENTRY_TYPE("blue", "󰌷 ", types.symlinks_f);
}

static void write_total_of_jobs()
{
    std::printf("%%(1j. %%F{magenta} %%f%%j.)");
}

int main()
{
    write_total_of_entry_types();
    write_total_of_jobs();
    std::printf("\n");
    return (0);
}
