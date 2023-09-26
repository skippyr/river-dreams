#include "Console.hpp"
#include "Directory.hpp"
#include "Shell.hpp"

#include <iostream>

using namespace RiverDreams::FileSystem;
using namespace RiverDreams::InputOutput;

static void WriteDirectoryEntryStatus(unsigned long status, std::string symbol, ConsoleColor color)
{
    if (status)
    {
        std::cout << Console::ApplyForegroundColor(symbol, color) << status;
    }
}

static void WriteTotalOfBackgroundJobs()
{
    std::string  totalOfBackgroundJobs    = Shell::GetTotalOfBackgroundJobs();
    std::string  backgroundJobSymbol      = "  ";
    ConsoleColor backgroundJobSymbolColor = ConsoleColor::Magenta;
    std::cout << Shell::WrapOnBackgroundJobEvent(
        Console::ApplyForegroundColor(backgroundJobSymbol, backgroundJobSymbolColor) +
            totalOfBackgroundJobs,
        "");
}

int main()
{
    DirectoryEntriesStatus entriesStatus = Directory(".").GetDirectoryEntriesStatus();
    WriteDirectoryEntryStatus(entriesStatus.totalOfHiddenEntries, "  ", ConsoleColor::Red);
    WriteDirectoryEntryStatus(entriesStatus.totalOfSymlinkEntries, " 󰌷 ", ConsoleColor::Blue);
    WriteDirectoryEntryStatus(entriesStatus.totalOfExecutableEntries, " 󱖏 ",
                              ConsoleColor::Green);
    WriteTotalOfBackgroundJobs();
    return EXIT_SUCCESS;
}
