#pragma once

#include "Color.hpp"

#include <string>

#include <sys/ioctl.h>

namespace RiverDreams
{
    class Terminal
    {
    public:
        static unsigned short GetWidth();
        static std::string ApplyForegroundColor(RiverDreams::Color, std::string text);
    };
}
