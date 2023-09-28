#pragma once

#include "Directory.hpp"
#include "EnvironmentVariables.hpp"
#include "File.hpp"

namespace RiverDreams::FileSystem::VersionControl
{
    class GitRepository
    {
    private:
        std::string rootDirectory;

        std::string SearchForRootDirectory(std::string searchDirectory =
                                               RiverDreams::Environment::EnvironmentVariables::GetPWD());

    public:
        GitRepository();
        std::string GetRootDirectory();
        std::string GetBranch();
    };
}
