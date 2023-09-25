#pragma once

#include "Directory.hpp"

#include <unistd.h>

namespace RiverDreams::Users
{
    class User
    {
    public:
        static bool IsRootUser();
        static bool OwnsCurrentDirectory();
    };
}
