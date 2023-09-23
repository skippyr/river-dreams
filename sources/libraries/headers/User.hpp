#pragma once

#include <sys/stat.h>
#include <unistd.h>

namespace RiverDreams
{
    class User
    {
    public:
        static bool IsRootUser();
        static bool OwnsCurrentDirectory();
    };
}
