#pragma once

#include "Color.hpp"

#include <string>

#include <sys/ioctl.h>

class Terminal
{
public:
    static unsigned short GetWidth();
    static std::string ApplyForegroundColor(Color color, std::string text);
};
