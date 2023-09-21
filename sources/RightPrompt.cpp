#include "DirectoryEntriesStatus.hpp"
#include "Shell.hpp"
#include "Terminal.hpp"

#include <iostream>

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
    std::string backgroundJobSymbol      = "  ";
    Color       backgroundJobSymbolColor = Color::MAGENTA;
    std::cout << Shell::WrapOnBackgroundJobEvent(
        Terminal::ApplyForegroundColor(backgroundJobSymbolColor, backgroundJobSymbol) + quantityOfBackgroundJobs, "");
}

int main()
{
    DirectoryEntriesStatus directoryEntriesStatus = DirectoryEntriesStatus(".");
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfHiddenEntries(), "  ", Color::RED);
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfSymlinkEntries(), " 󰌷 ", Color::BLUE);
    PrintDirectoryEntryStatus(directoryEntriesStatus.GetTotalOfExecutableEntries(), " 󱖏 ", Color::GREEN);
    PrintQuantityOfBackgroundJobs();
    std::cout << std::endl;
    return (EXIT_SUCCESS);
}
