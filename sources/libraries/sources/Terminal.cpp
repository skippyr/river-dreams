#include "Terminal.hpp"

unsigned short Terminal::GetWidth()
{
    struct winsize terminalProperties;
    int stderrFileDescriptor = 2;
    return ioctl(stderrFileDescriptor, TIOCGWINSZ, &terminalProperties) < 0 ? 0 : terminalProperties.ws_col;
}

std::string Terminal::ApplyForegroundColor(Color color, std::string text)
{
    return "%F{" + std::to_string((int)color) + "}" + text + "%f";
}
