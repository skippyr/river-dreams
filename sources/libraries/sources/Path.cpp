#include "Path.hpp"

using namespace RiverDreams;

std::string Path::GetBaseName(std::string path)
{
    return path.substr(path.find_last_of("/") + 1);
}

std::string Path::GetParentDirectory(std::string path)
{
    return path.substr(0, path.find_last_of("/"));
}

std::string Path::GetCurrentDirectoryAbbreviated(std::string repositoryRootDirectory)
{
    std::string pwd                 = EnvironmentVariables::GetPWD();
    std::string repositoryName      = GetBaseName(repositoryRootDirectory);
    std::string repositoryInnerPath = pwd.substr(repositoryRootDirectory.length());
    return repositoryRootDirectory != "" ? "@/" + repositoryName + repositoryInnerPath : "%~";
}
