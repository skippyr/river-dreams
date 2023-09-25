#pragma once

#include "EnvironmentVariables.hpp"

#include <string>

namespace RiverDreams::FileSystem
{
    class Path
    {
    public:
        static std::string GetBaseName(std::string path);
        static std::string GetParent(std::string path);
        static std::string GetCurrentDirectoryPathAbbreviated(std::string repositoryRootDirectoryPath);
    };
}
