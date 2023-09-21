#pragma once

#include <sys/stat.h>
#include <unistd.h>

class User
{
public:
    static bool IsRootUser();
    static bool OwnsCurrentDirectory();
};
