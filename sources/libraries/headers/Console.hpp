#pragma once

#include "ConsoleColor.hpp"

#include <string>

#include <sys/ioctl.h>

namespace RiverDreams::InputOutput
{
    class Console
    {
    public:
        static unsigned short GetTotalOfColumns();
        static std::string    ApplyForegroundColor(std::string text, ConsoleColor color);
    };
}
