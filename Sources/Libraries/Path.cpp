#include "Path.hpp"

#define PATH_SEPARATOR "/"
#define ROOT_DIRECTORY "/"

using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem;

Path::Path(std::string path)
{ this->path = path; }

Path Path::Join(std::string path)
{ return (Path(this->path + PATH_SEPARATOR + path)); }

std::string Path::ToString()
{ return (path); }

bool Path::IsRoot(std::string path)
{ return (path == ROOT_DIRECTORY); }

std::string Path::GetBase(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of(PATH_SEPARATOR);
    return (lastSlashIndex > path.length() ? path : path.substr(lastSlashIndex + 1));
}

std::string Path::GetParent(std::string path)
{
    unsigned long lastSlashIndex = path.find_last_of(PATH_SEPARATOR);
    return (!lastSlashIndex && path.length() > 1 ? ROOT_DIRECTORY : path.substr(0, lastSlashIndex));
}

std::string Path::GetPWDAbbreviated(std::string repositoryRootDirectory)
{
    std::string repositoryName      = GetBase(repositoryRootDirectory);
    std::string repositoryInnerPath = EnvironmentVariables::GetPWD().substr(repositoryRootDirectory
                                                                                .length());
    return (repositoryRootDirectory != "" ? "@/" + repositoryName + repositoryInnerPath : "%~");
}
