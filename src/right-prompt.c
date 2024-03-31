#include "right-prompt.h"

#define WRITE_ENTRY_TYPE_COUNT(a_color, a_symbol, a_value) \
    if (a_value) \
    { \
        printf(" %%F{" a_color "}" a_symbol "%%f%zu", a_value); \
    }

static void countEntryTypes(struct EntryTypesCount* count)
{
    memset(count, 0, sizeof(struct EntryTypesCount));
    DIR* directory = opendir(".");
    if (!directory)
    {
        return;
    }
    for (struct dirent* entry; (entry = readdir(directory));)
    {
        if (*entry->d_name == '.' && (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2])))
        {
            continue;
        }
        else if (entry->d_type == DT_REG)
        {
            ++count->regulars;
        }
        else if (entry->d_type == DT_DIR)
        {
            ++count->directories;
        }
        else if (entry->d_type == DT_BLK)
        {
            ++count->blocks;
        }
        else if (entry->d_type == DT_CHR)
        {
            ++count->characters;
        }
        else if (entry->d_type == DT_SOCK)
        {
            ++count->sockets;
        }
        else if (entry->d_type == DT_FIFO)
        {
            ++count->fifos;
        }
        else if (entry->d_type == DT_LNK)
        {
            ++count->symlinks;
        }
    }
    closedir(directory);
}

static void writeEntryTypesCountModule(void)
{
    struct EntryTypesCount count;
    countEntryTypes(&count);
    WRITE_ENTRY_TYPE_COUNT("blue", " ", count.regulars);
    WRITE_ENTRY_TYPE_COUNT("yellow", " ", count.directories);
    WRITE_ENTRY_TYPE_COUNT("magenta", "󰇖 ", count.blocks);
    WRITE_ENTRY_TYPE_COUNT("green", "󱣴 ", count.characters);
    WRITE_ENTRY_TYPE_COUNT("cyan", "󱄙 ", count.sockets);
    WRITE_ENTRY_TYPE_COUNT("blue", "󰟦 ", count.fifos);
    WRITE_ENTRY_TYPE_COUNT("blue", "󰌷 ", count.symlinks);
}

static void writeJobsCountModule(void)
{
    printf("%%(1j. %%F{magenta} %%f%%j.)");
}

int main(void)
{
    writeEntryTypesCountModule();
    writeJobsCountModule();
    putchar('\n');
    return 0;
}
