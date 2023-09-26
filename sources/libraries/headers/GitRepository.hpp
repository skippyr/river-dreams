#pragma once

#include "Directory.hpp"
#include "File.hpp"

namespace RiverDreams::FileSystem::VersionControl
{
    class GitRepository
    {
    private:
        std::string rootDirectoryPath;

        std::string SearchForRootDirectoryPath(std::string searchDirectoryPath);

    public:
        GitRepository();
        std::string GetRootDirectoryPath();
        std::string GetBranch();
    };
}
