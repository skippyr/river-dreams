#include "right-prompt.h"

static void countEntries(struct EntriesCount *count)
{
    memset(count, 0, sizeof(struct EntriesCount));
    int directory = open(".", O_RDONLY);
    if (!directory)
    {
        return;
    }
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer)
    {
        close(directory);
        return;
    }
    for (long totalEntries;
         (totalEntries = syscall(SYS_getdents64, directory, buffer, BUFFER_SIZE)) > 0;)
    {
        struct linux_dirent64 *entry;
        for (long offset = 0; offset < totalEntries; offset += entry->d_reclen)
        {
            entry = (struct linux_dirent64 *)(buffer + offset);
            if (entry->d_name[0] == '.')
            {
                if (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2]))
                {
                    continue;
                }
                ++count->totalHiddenEntries;
            }
            size_t length = strlen(entry->d_name);
            if (entry->d_name[length - 1] == '~')
            {
                ++count->totalTemporaryEntries;
            }
            struct stat status;
            switch (entry->d_type == DT_LNK && ++count->totalSymlinks &&
                            !stat(entry->d_name, &status)
                        ? status.st_mode & S_IFMT
                        : entry->d_type)
            {
            case S_IFREG:
            case DT_REG:
                ++count->totalFiles;
                break;
            case S_IFDIR:
            case DT_DIR:
                ++count->totalDirectories;
                break;
            case S_IFBLK:
            case DT_BLK:
                ++count->totalBlocks;
                break;
            case S_IFCHR:
            case DT_CHR:
                ++count->totalCharacters;
                break;
            case S_IFIFO:
            case DT_FIFO:
                ++count->totalFifos;
                break;
            case S_IFSOCK:
            case DT_SOCK:
                ++count->totalSockets;
                break;
            }
        }
    }
    free(buffer);
    close(directory);
}

static void writeEntryCount(const char *symbol, const char *color, int value)
{
    if (!value)
    {
        return;
    }
    color ? printf(" %%F{%s}%s%%f", color, symbol) : printf(" %s", symbol);
    char buffer[7];
    sprintf(buffer, "%d", value);
    int length = strlen(buffer);
    for (int separatorOffset = length - 3, offset = 0; offset < length; ++offset)
    {
        if (offset == separatorOffset && separatorOffset)
        {
            putchar(',');
        }
        putchar(buffer[offset]);
    }
}

static void writeEntryModule(void)
{
    struct EntriesCount count;
    countEntries(&count);
    writeEntryCount(" ", NULL, count.totalFiles);
    writeEntryCount("󰝰 ", "yellow", count.totalDirectories);
    writeEntryCount("󰌷 ", "blue", count.totalSymlinks);
    writeEntryCount("󰇖 ", "magenta", count.totalBlocks);
    writeEntryCount("󱣴 ", "green", count.totalCharacters);
    writeEntryCount("󰟦 ", "blue", count.totalFifos);
    writeEntryCount("󱄙 ", "cyan", count.totalSockets);
    writeEntryCount("󰈉 ", "red", count.totalHiddenEntries);
    writeEntryCount("󱣹 ", "magenta", count.totalTemporaryEntries);
}

static void writeJobModule(void)
{
    printf("%%(1j. %%F{magenta} %%f%%j.)");
}

int main(void)
{
    writeEntryModule();
    writeJobModule();
    putchar('\n');
}
