#include "Path.hpp"

using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem;

std::string Path::GetBaseName(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of("/");
    return lastSlashIndex > path.length() ? path : path.substr(lastSlashIndex + 1);
}

std::string Path::GetParent(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of("/");
    return !lastSlashIndex && path.length() > 1 ? "/" : path.substr(0, lastSlashIndex);
}

std::string Path::GetCurrentDirectoryPathAbbreviated(std::string repositoryRootDirectoryPath)
{
    std::string pwd                 = EnvironmentVariables::GetPWD();
    std::string repositoryName      = GetBaseName(repositoryRootDirectoryPath);
    std::string repositoryInnerPath = pwd.substr(repositoryRootDirectoryPath.length());
    return repositoryRootDirectoryPath != "" ? "@/" + repositoryName + repositoryInnerPath : "%~";
}
