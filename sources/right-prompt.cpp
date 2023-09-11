#include <sys/stat.h>

#include <dirent.h>

#include <cstring>
#include <iostream>

struct Directory_Entries_Status
{
    size_t total_of_hidden, total_of_symlinks, total_of_executables;
};

static void Get_Directory_Entries_Status(
    struct Directory_Entries_Status *status)
{
    std::memset(status, 0, sizeof(struct Directory_Entries_Status));
    DIR *stream = opendir(".");
    if (!stream)
    {
        return;
    }
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if (!std::strcmp(entry->d_name, ".") ||
            !std::strcmp(entry->d_name, ".."))
        {
            continue;
        }
        std::string path = "./" + std::string(entry->d_name);
        struct stat metadata;
        if (lstat(path.c_str(), &metadata))
        {
            continue;
        }
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

static void Print_Directory_Entry_Status(size_t status, int color,
                                         std::string symbol)
{
    if (status)
    {
        std::cout << " %F{" << color << "}" << symbol << "%f" << status;
    }
}

static void Print_Jobs_Status()
{
    std::cout << "%(1j. %F{5} %f %j.)";
}

int main()
{
    Directory_Entries_Status status;
    Get_Directory_Entries_Status(&status);
    Print_Directory_Entry_Status(status.total_of_hidden, 1, " ");
    Print_Directory_Entry_Status(status.total_of_executables, 2, "󱖏 ");
    Print_Directory_Entry_Status(status.total_of_symlinks, 4, "󰌷 ");
    Print_Jobs_Status();
    std::cout << std::endl;
    return EXIT_SUCCESS;
}
