#include "right-prompt.hpp"

static struct EntriesCount CountEntriesInCWD()
{
    struct EntriesCount count;
    int stream = open(".", O_RDONLY);
    if (stream < 0)
    {
        return count;
    }
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUFFER_SIZE);
    for (std::size_t totalEntries;
         (totalEntries = syscall(SYS_getdents64, stream, buffer.get(), BUFFER_SIZE)) > 0;)
    {
        struct linux_dirent64* entry;
        for (std::size_t offset = 0; offset < totalEntries; offset += entry->d_reclen)
        {
            entry = reinterpret_cast<struct linux_dirent64*>(buffer.get() + offset);
            if (entry->d_name[0] == '.')
            {
                if (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2]))
                {
                    continue;
                }
                ++count.totalHiddenEntries;
            }
            std::size_t length = std::strlen(entry->d_name);
            if (entry->d_name[length - 1] == '~')
            {
                ++count.totalTemporaryEntries;
            }
            struct stat status;
            switch (entry->d_type == DT_LNK && ++count.totalSymlinks &&
                            !stat(entry->d_name, &status)
                        ? status.st_mode & S_IFMT
                        : entry->d_type)
            {
            case S_IFREG:
            case DT_REG:
                ++count.totalFiles;
                break;
            case S_IFDIR:
            case DT_DIR:
                ++count.totalDirectories;
                break;
            case S_IFBLK:
            case DT_BLK:
                ++count.totalBlocks;
                break;
            case S_IFCHR:
            case DT_CHR:
                ++count.totalCharacters;
                break;
            case S_IFIFO:
            case DT_FIFO:
                ++count.totalFifos;
                break;
            case S_IFSOCK:
            case DT_SOCK:
                ++count.totalSockets;
                break;
            }
        }
    }
    close(stream);
    return count;
}

static void WriteEntriesCountModule()
{
    struct EntriesCount count = CountEntriesInCWD();
    WriteEntryCount(" ", nullptr, count.totalFiles);
    WriteEntryCount("󰝰 ", "yellow", count.totalDirectories);
    WriteEntryCount("󰌷 ", "blue", count.totalSymlinks);
    WriteEntryCount("󰇖 ", "magenta", count.totalBlocks);
    WriteEntryCount("󱣴 ", "green", count.totalCharacters);
    WriteEntryCount("󰟦 ", "blue", count.totalFifos);
    WriteEntryCount("󱄙 ", "cyan", count.totalSockets);
    WriteEntryCount("󰈉 ", "red", count.totalHiddenEntries);
    WriteEntryCount("󱣹 ", "magenta", count.totalTemporaryEntries);
}

static void WriteEntryCount(const char* symbol, const char* color, int count)
{
    if (!count)
    {
        return;
    }
    if (color)
    {
        std::cout << " %F{" << color << "}" << symbol << "%f";
    }
    else
    {
        std::cout << symbol;
    }
    char buffer[7];
    std::sprintf(buffer, "%d", count);
    for (int length = std::strlen(buffer), separatorOffset = length - 3, offset = 0;
         offset < length; ++offset)
    {
        if (offset == separatorOffset && separatorOffset)
        {
            std::cout << ",";
        }
        std::cout << buffer[offset];
    }
}

static void WriteJobsModule()
{
    std::cout << "%(1j. %F{magenta} %f%j.)";
}

int main()
{
    WriteEntriesCountModule();
    WriteJobsModule();
    std::cout << std::endl;
    return 0;
}
