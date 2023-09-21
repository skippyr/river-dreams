#pragma once

#include "Path.hpp"

#include <cstring>

#include <dirent.h>

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
