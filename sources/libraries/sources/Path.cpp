#include "Path.hpp"

#define PATH_SEPARATOR "/"

using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem;

Path::Path(std::string path)
{
    this->path = path;
}

Path Path::Join(std::string path)
{
    this->path += PATH_SEPARATOR + path;
    return Path(this->path);
}

std::string Path::ToString()
{
    return path;
}

std::string Path::GetBase(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of("/");
    return lastSlashIndex > path.length() ? path : path.substr(lastSlashIndex + 1);
}

std::string Path::GetParent(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of("/");
    return !lastSlashIndex && path.length() > 1 ? "/" : path.substr(0, lastSlashIndex);
}

std::string Path::GetPWDAbbreviated(std::string repositoryRootDirectoryPath)
{
    std::string repositoryName      = GetBase(repositoryRootDirectoryPath);
    std::string repositoryInnerPath = EnvironmentVariables::GetPWD()
                                          .substr(repositoryRootDirectoryPath.length());
    return repositoryRootDirectoryPath != "" ? "@/" + repositoryName + repositoryInnerPath : "%~";
}
