#include "DirectoryEntriesStatus.hpp"

DirectoryEntriesStatus::DirectoryEntriesStatus(std::string path)
{
    DIR* directoryStream = opendir(path.c_str());
    if (!directoryStream)
    {
        return;
    }
    for (struct dirent* directoryEntry; (directoryEntry = readdir(directoryStream));)
    {
        if (!std::strcmp(directoryEntry->d_name, ".") || !std::strcmp(directoryEntry->d_name, ".."))
        {
            continue;
        }
        struct stat directoryEntryProperties;
        if (lstat(directoryEntry->d_name, &directoryEntryProperties))
        {
            continue;
        }
        if (S_ISLNK(directoryEntryProperties.st_mode))
        {
            this->totalOfSymlinkEntries++;
        }
        else if (S_ISREG(directoryEntryProperties.st_mode) && directoryEntryProperties.st_mode & S_IXUSR)
        {
            this->totalOfExecutableEntries++;
        }
        if (directoryEntry->d_name[0] == '.')
        {
            this->totalOfHiddenEntries++;
        }
    }
    closedir(directoryStream);
}

unsigned long DirectoryEntriesStatus::GetTotalOfExecutableEntries()
{
    return (this->totalOfExecutableEntries);
}

unsigned long DirectoryEntriesStatus::GetTotalOfSymlinkEntries()
{
    return (this->totalOfSymlinkEntries);
}

unsigned long DirectoryEntriesStatus::GetTotalOfHiddenEntries()
{
    return (this->totalOfHiddenEntries);
}
