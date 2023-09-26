#pragma once

#include "DirectoryEntriesStatus.hpp"
#include "Path.hpp"

#include <string>

#include <dirent.h>
#include <sys/stat.h>

namespace RiverDreams::FileSystem
{
    class Directory
    {
    private:
        std::string path;
        DIR*        stream    = 0;
        bool        hasOpened = false;

    public:
        Directory(std::string path);
        ~Directory();
        void                   OpenStream();
        bool                   IsRepositoryRoot();
        DirectoryEntriesStatus GetEntriesStatus();
        unsigned               GetUserId();
    };
}
