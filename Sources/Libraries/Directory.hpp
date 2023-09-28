#pragma once

#include "DirectoryEntriesStatus.hpp"
#include "Path.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <string>

namespace RiverDreams::FileSystem
{
    class Directory
    {
    private:
        std::string path;
        DIR*        stream = 0;

        bool OpenStream();

    public:
        Directory(std::string path);
        ~Directory();
        unsigned               GetUserIdentifier();
        bool                   IsRepositoryRoot();
        DirectoryEntriesStatus GetEntriesStatus();
    };
}
