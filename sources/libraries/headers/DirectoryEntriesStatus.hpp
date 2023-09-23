#pragma once

#include <cstring>
#include <string>

#include <dirent.h>
#include <sys/stat.h>

class DirectoryEntriesStatus
{
private:
    unsigned long totalOfExecutableEntries = 0;
    unsigned long totalOfSymlinkEntries = 0;
    unsigned long totalOfHiddenEntries = 0;

public:
    DirectoryEntriesStatus(std::string path);
    unsigned long GetTotalOfExecutableEntries();
    unsigned long GetTotalOfSymlinkEntries();
    unsigned long GetTotalOfHiddenEntries();
};
