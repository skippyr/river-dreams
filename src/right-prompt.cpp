#include <dirent.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdio>

#define WRITE_ENTRY_TYPE(color_m, symbol_m, value_m) \
    if (value_m) { \
        std::printf(" %%F{%s}%s%%f%ld", color_m, symbol_m, value_m); \
    }

struct EntryTypes
{
    std::size_t regulars_s;
    std::size_t directories_s;
    std::size_t blocks_s;
    std::size_t characters_s;
    std::size_t sockets_s;
    std::size_t fifos_s;
    std::size_t symlinks_s;
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
                types.regulars_s++;
            } else if (entry->d_type == DT_DIR) {
                types.directories_s++;
            } else if (entry->d_type == DT_BLK) {
                types.blocks_s++;
            } else if (entry->d_type == DT_CHR) {
                types.characters_s++;
            } else if (entry->d_type == DT_SOCK) {
                types.sockets_s++;
            } else if (entry->d_type == DT_FIFO) {
                types.fifos_s++;
            } else if (entry->d_type == DT_LNK) {
                types.symlinks_s++;
            }
        }
    }
    close(directory);
    return (types);
}

static void write_total_of_entry_types()
{
    struct EntryTypes types = count_entry_types();
    WRITE_ENTRY_TYPE("blue", " ", types.regulars_s);
    WRITE_ENTRY_TYPE("yellow", " ", types.directories_s);
    WRITE_ENTRY_TYPE("magenta", "󰇖 ", types.blocks_s);
    WRITE_ENTRY_TYPE("green", "󱣴 ", types.characters_s);
    WRITE_ENTRY_TYPE("cyan", "󱄙 ", types.sockets_s);
    WRITE_ENTRY_TYPE("blue", "󰟦 ", types.fifos_s);
    WRITE_ENTRY_TYPE("blue", "󰌷 ", types.symlinks_s);
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
