#include "Console.hpp"
#include "GitRepository.hpp"
#include "Network.hpp"
#include "Shell.hpp"
#include "StorageDevice.hpp"
#include "SystemTime.hpp"
#include "User.hpp"

#include <iostream>

#define CHEVRON_LEFT_DELIMITER       ":«("
#define CHEVRON_RIGHT_DELIMITER      ")»:"
#define PARENTHESIS_LEFT_DELIMITER   "("
#define PARENTHESIS_RIGHT_DELIMITER  ")"
#define CURLY_BRACES_LEFT_DELIMITER  "{"
#define CURLY_BRACES_RIGHT_DELIMITER "}"

using namespace RiverDreams::Connectivity;
using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem;
using namespace RiverDreams::FileSystem::VersionControl;
using namespace RiverDreams::InputOutput;
using namespace RiverDreams::Time;
using namespace RiverDreams::Users;

static void WriteCommandsSeparator()
{
    for (unsigned short column = 0; column < Console::GetTotalOfColumns(); column++)
    {
        bool         isColumnOdd          = column % 2;
        std::string  separatorSymbol      = isColumnOdd ? "⊼" : "⊵";
        ConsoleColor separatorSymbolColor = isColumnOdd ? ConsoleColor::Red : ConsoleColor::Yellow;
        std::cout << Console::ApplyForegroundColor(separatorSymbol, separatorSymbolColor);
    }
    std::cout << std::endl;
}

static void WriteHorizontalSpacing()
{
    std::cout << "  ";
}

static void WriteLocalIPV4Address()
{
    std::string  networkSymbol   = " ";
    std::string  separatorSymbol = "@";
    ConsoleColor symbolsColor    = ConsoleColor::Blue;
    std::cout << Console::ApplyForegroundColor(networkSymbol, symbolsColor) << Network::GetHostName()
              << Console::ApplyForegroundColor(separatorSymbol, symbolsColor) << Network::GetLocalIPV4Address();
}

static void WriteStorageDeviceUsagePercentage()
{
    std::string  storageDeviceSymbol      = "󰋊 ";
    ConsoleColor storageDeviceSymbolColor = ConsoleColor::Yellow;
    std::cout << Console::ApplyForegroundColor(storageDeviceSymbol, storageDeviceSymbolColor)
              << StorageDevice::GetUsagePercentage() << "%%";
}

static void WriteCalendar(SystemTime& systemTime)
{
    std::string  calendarSymbol      = "󰸗 ";
    ConsoleColor calendarSymbolColor = ConsoleColor::Red;
    std::cout << Console::ApplyForegroundColor(calendarSymbol, calendarSymbolColor) << systemTime.GetCalendar();
}

static void WriteClock(SystemTime& systemTime)
{
    std::string  clockSymbol;
    ConsoleColor clockSymbolColor;
    switch (systemTime.GetDayMoment())
    {
    case DayMoment::Dawn:
        clockSymbol      = "󰭎 ";
        clockSymbolColor = ConsoleColor::Magenta;
        break;
    case DayMoment::Morning:
        clockSymbol      = "󰖨 ";
        clockSymbolColor = ConsoleColor::Red;
        break;
    case DayMoment::Afternoon:
        clockSymbol      = " ";
        clockSymbolColor = ConsoleColor::Blue;
        break;
    case DayMoment::Night:
        clockSymbol      = "󰽥 ";
        clockSymbolColor = ConsoleColor::Yellow;
        break;
    default:
        clockSymbol      = " ";
        clockSymbolColor = ConsoleColor::Blue;
    }
    std::cout << Console::ApplyForegroundColor(clockSymbol, clockSymbolColor) << systemTime.GetClock();
}

static void WriteRootStatus()
{
    std::string  rootSymbol      = "#";
    ConsoleColor rootSymbolColor = ConsoleColor::Red;
    ConsoleColor delimitersColor = ConsoleColor::Yellow;
    if (User::IsRootUser())
    {
        std::cout << Console::ApplyForegroundColor(CURLY_BRACES_LEFT_DELIMITER, delimitersColor)
                  << Console::ApplyForegroundColor(rootSymbol, rootSymbolColor)
                  << Console::ApplyForegroundColor(CURLY_BRACES_RIGHT_DELIMITER, delimitersColor);
    }
}

static void WriteExitCodeStatus()
{
    std::string  successSymbol      = "≗";
    std::string  errorSymbol        = "⨲";
    ConsoleColor successSymbolColor = ConsoleColor::Yellow;
    ConsoleColor errorSymbolColor   = ConsoleColor::Red;
    ConsoleColor delimitersColor    = ConsoleColor::Yellow;
    std::cout << Console::ApplyForegroundColor(CURLY_BRACES_LEFT_DELIMITER, delimitersColor)
              << Shell::WrapOnErrorEvent(Console::ApplyForegroundColor(errorSymbol, errorSymbolColor),
                                         Console::ApplyForegroundColor(successSymbol, successSymbolColor))
              << Console::ApplyForegroundColor(CURLY_BRACES_RIGHT_DELIMITER, delimitersColor);
}

static void WriteVirtualEnv()
{
    std::string  virtualEnvName  = Path::GetBaseName(EnvironmentVariables::GetVirtualEnv());
    ConsoleColor delimitersColor = ConsoleColor::Magenta;
    if (virtualEnvName != "")
    {
        std::cout << Console::ApplyForegroundColor(PARENTHESIS_LEFT_DELIMITER, delimitersColor) << virtualEnvName
                  << Console::ApplyForegroundColor(PARENTHESIS_RIGHT_DELIMITER, delimitersColor) << " ";
    }
}

static void WriteCurrentDirectoryPathAbbreviated(GitRepository& gitRepository)
{
    ConsoleColor pathColor = ConsoleColor::Red;
    std::cout << Console::ApplyForegroundColor(
        Path::GetCurrentDirectoryPathAbbreviated(gitRepository.GetRootDirectoryPath()), pathColor);
}

static void WriteGitRepositoryBranch(GitRepository& gitRepository)
{
    std::string  branch          = gitRepository.GetBranch();
    ConsoleColor delimitersColor = ConsoleColor::Green;
    if (branch != "")
    {
        std::cout << Console::ApplyForegroundColor(CHEVRON_LEFT_DELIMITER, delimitersColor) << branch
                  << Console::ApplyForegroundColor(CHEVRON_RIGHT_DELIMITER, delimitersColor);
    }
}

static void WriteCurrentDirectoryOwnership()
{
    std::string  readOnlySymbol      = " ";
    ConsoleColor readOnlySymbolColor = ConsoleColor::Magenta;
    if (!User::OwnsCurrentDirectory())
    {
        std::cout << Console::ApplyForegroundColor(readOnlySymbol, readOnlySymbolColor);
    }
}

int main()
{
    SystemTime    systemTime        = SystemTime();
    GitRepository gitRepository     = GitRepository();
    std::string   arrowSymbol       = "⤐  ";
    std::string   cursorSymbol      = " ⩺ ";
    ConsoleColor  arrowSymbolColor  = ConsoleColor::Yellow;
    ConsoleColor  cursorSymbolColor = ConsoleColor::Magenta;
    ConsoleColor  delimitersColor   = ConsoleColor::Yellow;
    WriteCommandsSeparator();
    std::cout << Console::ApplyForegroundColor(CHEVRON_LEFT_DELIMITER, delimitersColor);
    WriteLocalIPV4Address();
    WriteHorizontalSpacing();
    WriteStorageDeviceUsagePercentage();
    WriteHorizontalSpacing();
    WriteCalendar(systemTime);
    WriteHorizontalSpacing();
    WriteClock(systemTime);
    std::cout << Console::ApplyForegroundColor(CHEVRON_RIGHT_DELIMITER, delimitersColor) << std::endl;
    WriteRootStatus();
    WriteExitCodeStatus();
    std::cout << Console::ApplyForegroundColor(arrowSymbol, arrowSymbolColor);
    WriteVirtualEnv();
    WriteCurrentDirectoryPathAbbreviated(gitRepository);
    WriteGitRepositoryBranch(gitRepository);
    WriteCurrentDirectoryOwnership();
    std::cout << Console::ApplyForegroundColor(cursorSymbol, cursorSymbolColor) << std::endl;
    return EXIT_SUCCESS;
}
