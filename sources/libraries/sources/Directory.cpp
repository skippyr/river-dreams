#include "Directory.hpp"

#define ROOT_UID 1

using namespace RiverDreams::FileSystem;

Directory::Directory(std::string path)
{ this->path = path; }

Directory::~Directory()
{
    if (stream)
    { closedir(stream); }
}

void Directory::OpenStream()
{
    if (!hasOpened)
    {
        stream    = opendir(path.c_str());
        hasOpened = true;
    }
}

bool Directory::IsRepositoryRoot()
{
    OpenStream();
    if (!stream)
    { return false; }
    rewinddir(stream);
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if ((std::string)entry->d_name == ".git")
        { return true; }
    }
    return false;
}

DirectoryEntriesStatus Directory::GetEntriesStatus()
{
    DirectoryEntriesStatus entriesStatus = DirectoryEntriesStatus();
    OpenStream();
    if (!stream)
    { return entriesStatus; }
    rewinddir(stream);
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        std::string entryName = entry->d_name;
        std::string entryPath = Path(path).Join(entryName).ToString();
        struct stat entryProperties;
        if (entryName == "." || entryName == ".." || lstat(entryPath.c_str(), &entryProperties))
        { continue; }
        if (entryName[0] == '.')
        { entriesStatus.totalOfHiddenEntries++; }
        if (S_ISLNK(entryProperties.st_mode))
        { entriesStatus.totalOfSymlinkEntries++; }
        else if (S_ISREG(entryProperties.st_mode) && entryProperties.st_mode & S_IXUSR)
        { entriesStatus.totalOfExecutableEntries++; }
    }
    return entriesStatus;
}

unsigned Directory::GetUserId()
{
    struct stat properties;
    return stat(path.c_str(), &properties) ? ROOT_UID : properties.st_uid;
}
