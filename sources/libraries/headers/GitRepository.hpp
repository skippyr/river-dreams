#pragma once

#include "Directory.hpp"
#include "File.hpp"
#include "Path.hpp"

namespace RiverDreams::FileSystem::VersionControl
{
    class GitRepository
    {
    private:
        std::string rootDirectoryPath;

        std::string SearchForRootDirectoryPath(std::string searchPath);

    public:
        GitRepository();
        std::string GetRootDirectoryPath();
        std::string GetBranch();
    };
}
