#include "User.hpp"

bool User::IsRootUser()
{
    return !getuid();
}

bool User::OwnsCurrentDirectory()
{
    unsigned short uid = getuid();
    struct stat currentDirectoryProperties;
    return lstat(".", &currentDirectoryProperties) ? false : uid && currentDirectoryProperties.st_uid != uid;
}
