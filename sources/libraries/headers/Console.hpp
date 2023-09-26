#pragma once

#include "ConsoleColor.hpp"

#include <iostream>

#include <sys/ioctl.h>

namespace RiverDreams::IO
{
    class Console
    {
    public:
        static unsigned short GetTotalOfColumns();
        static std::string    ApplyForegroundColor(std::string  text  = "",
                                                   ConsoleColor color = ConsoleColor::None);
        static void           Write(std::string  text  = "",
                                    ConsoleColor color = ConsoleColor::None);
        static void           WriteLine(std::string  text  = "",
                                        ConsoleColor color = ConsoleColor::None);
    };
}
