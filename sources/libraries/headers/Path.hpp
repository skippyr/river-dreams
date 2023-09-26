#pragma once

#include "EnvironmentVariables.hpp"

#include <string>

namespace RiverDreams::FileSystem
{
    class Path
    {
    private:
        std::string path;

    public:
        Path(std::string path);
        Path Join(std::string path);
        std::string ToString();
        static std::string GetBase(std::string path);
        static std::string GetParent(std::string path);
        static std::string GetPWDAbbreviated(std::string repositoryRootDirectoryPath);
    };
}
