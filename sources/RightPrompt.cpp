#include "DirectoryEntriesStatus.hpp"
#include "Shell.hpp"
#include "Terminal.hpp"

#include <iostream>

using namespace RiverDreams;

static void PrintDirectoryEntryStatus(unsigned long status, std::string statusSymbol, Color statusSymbolColor)
{
    if (status)
    {
        std::cout << Terminal::ApplyForegroundColor(statusSymbolColor, statusSymbol) << status;
    }
}

static void PrintQuantityOfBackgroundJobs()
{
    std::string quantityOfBackgroundJobs = Shell::GetQuantityOfBackgroundJobs();
    std::string backgroundJobSymbol = "  ";
    Color backgroundJobSymbolColor = Color::Magenta;
    std::cout << Shell::WrapOnBackgroundJobEvent(
        Terminal::ApplyForegroundColor(backgroundJobSymbolColor, backgroundJobSymbol) + quantityOfBackgroundJobs, "");
}

int main()
{
    DirectoryEntriesStatus directoryEntriesStatus = DirectoryEntriesStatus(".");
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfHiddenEntries(), "  ", Color::Red);
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfSymlinkEntries(), " 󰌷 ", Color::Blue);
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfExecutableEntries(), " 󱖏 ", Color::Green);
    PrintQuantityOfBackgroundJobs();
    std::cout << std::endl;
    return EXIT_SUCCESS;
}
