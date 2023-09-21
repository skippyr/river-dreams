#include "Terminal.hpp"

unsigned short Terminal::GetWidth()
{
    struct winsize terminalProperties;
    int            stderrFileDescriptor = 2;
    return (ioctl(stderrFileDescriptor, TIOCGWINSZ, &terminalProperties) < 0 ? 0 : terminalProperties.ws_col);
}

int Terminal::GetColorAnsiCode(Color color)
{
    switch (color)
    {
    case Color::BLACK:
        return (0);
    case Color::RED:
        return (1);
    case Color::GREEN:
        return (2);
    case Color::YELLOW:
        return (3);
    case Color::BLUE:
        return (4);
    case Color::MAGENTA:
        return (5);
    case Color::CYAN:
        return (6);
    case Color::WHITE:
        return (7);
    default:
        return (7);
    }
}

std::string Terminal::ApplyForegroundColor(Color color, std::string text)
{
    return ("%F{" + std::to_string(GetColorAnsiCode(color)) + "}" + text + "%f");
}
