#include "Console.hpp"

#define STANDARD_ERROR_STREAM_FILE_DESCRIPTOR 2
#define GET_WINDOW_SIZE_OPERATION             TIOCGWINSZ

using namespace RiverDreams::IO;

unsigned short Console::GetTotalOfColumns()
{
    struct winsize windowProperties;
    return (ioctl(STANDARD_ERROR_STREAM_FILE_DESCRIPTOR, GET_WINDOW_SIZE_OPERATION,
                  &windowProperties) < 0
                ? 0
                : windowProperties.ws_col);
}

std::string Console::ApplyForegroundColor(std::string  text,
                                          ConsoleColor color)
{
    return (color == ConsoleColor::None ? text : "%F{" + std::to_string((int)color - 1) + "}"
                                                       + text + "%f");
}

void Console::Write(std::string  text,
                    ConsoleColor color)
{ std::cout << ApplyForegroundColor(text, color); }

void Console::WriteLine(std::string  text,
                        ConsoleColor color)
{
    Write(text, color);
    std::cout << std::endl;
}
