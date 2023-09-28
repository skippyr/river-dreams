#include "Console.hpp"
#include "Directory.hpp"
#include "Shell.hpp"

using namespace RiverDreams::FileSystem;
using namespace RiverDreams::IO;

static void WriteEntryStatus(unsigned long status,
                             std::string   symbol,
                             ConsoleColor  color);
static void WriteTotalOfBackgroundJobs();

static void WriteEntryStatus(unsigned long status,
                             std::string   symbol,
                             ConsoleColor  color)
{
    if (status)
    {
        Console::Write(symbol, color);
        Console::Write(std::to_string(status));
    }
}

static void WriteTotalOfBackgroundJobs()
{
    std::string  symbol = "  ";
    ConsoleColor color  = ConsoleColor::Magenta;
    Console::Write(Shell::WrapOnBackgroundJobEvent(Console::ApplyForegroundColor(symbol, color) +
                                                   Shell::GetTotalOfBackgroundJobs()));
}

int main()
{
    DirectoryEntriesStatus entriesStatus = Directory(".").GetEntriesStatus();
    WriteEntryStatus(entriesStatus.totalOfHiddenEntries, "  ", ConsoleColor::Red);
    WriteEntryStatus(entriesStatus.totalOfSymlinkEntries, " 󰌷 ", ConsoleColor::Blue);
    WriteEntryStatus(entriesStatus.totalOfExecutableEntries, " 󱖏 ", ConsoleColor::Green);
    WriteTotalOfBackgroundJobs();
    Console::WriteLine();
    return (EXIT_SUCCESS);
}
