#include "User.hpp"

bool User::IsRootUser()
{
    return (!getuid());
}

bool User::OwnsCurrentDirectory()
{
    unsigned short uid = getuid();
    struct stat    currentDirectoryProperties;
    if (lstat(".", &currentDirectoryProperties))
    {
        return (false);
    }
    return (uid && currentDirectoryProperties.st_uid != uid);
}
