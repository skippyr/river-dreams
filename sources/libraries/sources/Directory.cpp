#include "Directory.hpp"

#define ROOT_ID 1

using namespace RiverDreams::FileSystem;

Directory::Directory(std::string path)
{
    this->path = path;
    stream     = opendir(path.c_str());
}

Directory::~Directory()
{
    if (stream)
    {
        closedir(stream);
    }
}

bool Directory::IsRepositoryRoot()
{
    if (!stream)
    {
        return false;
    }
    rewinddir(stream);
    for (struct dirent* entry; (entry = readdir(stream));)
    {
        std::string entryName = entry->d_name;
        if (entryName == ".git")
        {
            return true;
        }
    }
    return false;
}

unsigned int Directory::GetUserId()
{
    struct stat directoryProperties;
    if (stat(path.c_str(), &directoryProperties))
    {
        return ROOT_ID;
    }
    return directoryProperties.st_uid;
}

DirectoryEntriesStatus Directory::GetDirectoryEntriesStatus()
{
    DirectoryEntriesStatus entriesStatus = DirectoryEntriesStatus();
    if (!stream)
    {
        return entriesStatus;
    }
    rewinddir(stream);
    for (struct dirent* entry; (entry = readdir(stream));)
    {
        std::string entryName = entry->d_name;
        std::string entryPath = path + "/" + entryName;
        struct stat entryProperties;
        if (entryName == "." || entryName == ".." || lstat(entryPath.c_str(), &entryProperties))
        {
            continue;
        }
        if (entryName[0] == '.')
        {
            entriesStatus.totalOfHiddenEntries++;
        }
        if (S_ISLNK(entryProperties.st_mode))
        {
            entriesStatus.totalOfSymlinkEntries++;
        }
        else if (S_ISREG(entryProperties.st_mode) && entryProperties.st_mode & S_IXUSR)
        {
            entriesStatus.totalOfExecutableEntries++;
        }
    }
    return entriesStatus;
}
