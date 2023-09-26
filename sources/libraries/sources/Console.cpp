#include "Console.hpp"

#define STANDARD_ERROR_FILE_DESCRIPTOR    2
#define GET_CONSOLE_WINDOW_SIZE_OPERATION TIOCGWINSZ

using namespace RiverDreams::InputOutput;

unsigned short Console::GetTotalOfColumns()
{
    struct winsize consoleWindowProperties;
    return ioctl(STANDARD_ERROR_FILE_DESCRIPTOR, GET_CONSOLE_WINDOW_SIZE_OPERATION,
                 &consoleWindowProperties) < 0
               ? 0
               : consoleWindowProperties.ws_col;
}

std::string Console::ApplyForegroundColor(std::string text, ConsoleColor color)
{
    return "%F{" + std::to_string((int)color) + "}" + text + "%f";
}
