#pragma once

#include "Directory.hpp"

#include <unistd.h>

namespace RiverDreams::Users
{
    class User
    {
    public:
        static bool IsRoot();
        static bool OwnsPWD();
    };
}
