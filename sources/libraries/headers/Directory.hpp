#pragma once

#include "DirectoryEntriesStatus.hpp"

#include <string>

#include <dirent.h>
#include <sys/stat.h>

namespace RiverDreams::FileSystem
{
    class Directory
    {
    private:
        std::string path;
        DIR*        stream;

    public:
        Directory(std::string path);
        ~Directory();
        bool                   IsRepositoryRoot();
        unsigned int           GetUserId();
        DirectoryEntriesStatus GetDirectoryEntriesStatus();
    };
}
