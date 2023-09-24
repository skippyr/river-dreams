#include "GitRepository.hpp"
#include "Network.hpp"
#include "Path.hpp"
#include "Shell.hpp"
#include "StorageDevice.hpp"
#include "SystemTime.hpp"
#include "Terminal.hpp"
#include "User.hpp"

#include <iostream>

#define CHEVRONS_LEFT_DELIMITER      ":«("
#define CHEVRONS_RIGHT_DELIMITER     ")»:"
#define CURLY_BRACES_LEFT_DELIMITER  "{"
#define CURLY_BRACES_RIGHT_DELIMITER "}"

using namespace RiverDreams;

static void PrintCommandsSeparator()
{
    for (unsigned short column = 0; column < Terminal::GetWidth(); column++)
    {
        bool        isColumnOdd = column % 2;
        std::string symbol      = isColumnOdd ? "⊼" : "⊵";
        Color       symbolColor = isColumnOdd ? Color::Red : Color::Yellow;
        std::cout << Terminal::ApplyForegroundColor(symbolColor, symbol);
    }
    std::cout << std::endl;
}

static void PrintWidgetsSeparator()
{
    std::cout << "  ";
}

static void PrintLocalIPV4Address()
{
    std::string localAddress    = Network::GetLocalIPV4Address();
    std::string hostName        = Network::GetHostName();
    std::string separatorSymbol = "@";
    std::string networkSymbol   = " ";
    Color       symbolsColor    = Color::Blue;
    std::cout << Terminal::ApplyForegroundColor(symbolsColor, networkSymbol) << hostName
              << Terminal::ApplyForegroundColor(symbolsColor, separatorSymbol) << localAddress;
}

static void PrintStorageDeviceUsagePercentage()
{
    unsigned short usagePercentage = StorageDevice::GetUsagePercentage();
    std::string    diskSymbol      = "󰋊 ";
    Color          diskSymbolColor = Color::Yellow;
    std::cout << Terminal::ApplyForegroundColor(diskSymbolColor, diskSymbol) << usagePercentage << "%%";
}

static void PrintCalendar(SystemTime& systemTime)
{
    std::string calendar            = systemTime.GetCalendar();
    std::string calendarSymbol      = "󰸗 ";
    Color       calendarSymbolColor = Color::Red;
    std::cout << Terminal::ApplyForegroundColor(calendarSymbolColor, calendarSymbol) << calendar;
}

static void PrintClock(SystemTime& systemTime)
{
    std::string clock              = systemTime.GetClock();
    std::string clockColoredSymbol = systemTime.GetColoredClockSymbol();
    std::cout << clockColoredSymbol << clock;
}

static void PrintExitCodeStatus()
{
    std::string errorSymbol        = "⨲";
    std::string successSymbol      = "≗";
    Color       errorSymbolColor   = Color::Red;
    Color       successSymbolColor = Color::Yellow;
    Color       delimitersColor    = Color::Yellow;
    std::cout << Terminal::ApplyForegroundColor(delimitersColor, CURLY_BRACES_LEFT_DELIMITER)
              << Shell::WrapOnErrorEvent(Terminal::ApplyForegroundColor(errorSymbolColor, errorSymbol),
                                         Terminal::ApplyForegroundColor(successSymbolColor, successSymbol))
              << Terminal::ApplyForegroundColor(delimitersColor, CURLY_BRACES_RIGHT_DELIMITER);
}

static void PrintRootStatus()
{
    std::string rootSymbol      = "#";
    Color       delimitersColor = Color::Yellow;
    Color       rootSymbolColor = Color::Red;
    if (User::IsRootUser())
    {
        std::cout << Terminal::ApplyForegroundColor(delimitersColor, CURLY_BRACES_LEFT_DELIMITER)
                  << Terminal::ApplyForegroundColor(rootSymbolColor, rootSymbol)
                  << Terminal::ApplyForegroundColor(delimitersColor, CURLY_BRACES_RIGHT_DELIMITER);
    }
}

static void PrintVirtualEnvironment()
{
    std::string virtualEnvironment     = EnvironmentVariables::GetVirtualEnvironment();
    std::string virtualEnvironmentName = Path::GetBaseName(virtualEnvironment);
    std::string leftDelimiter          = "(";
    std::string rightDelimiter         = ") ";
    Color       delimitersColor        = Color::Magenta;
    if (virtualEnvironmentName != "")
    {
        std::cout << Terminal::ApplyForegroundColor(delimitersColor, leftDelimiter) << virtualEnvironmentName
                  << Terminal::ApplyForegroundColor(delimitersColor, rightDelimiter);
    }
}

static void PrintCurrentDirectoryPath(GitRepository& repository)
{
    std::string currentDirectoryPath      = Path::GetCurrentDirectoryAbbreviated(repository.GetRootDirectoryPath());
    Color       currentDirectoryPathColor = Color::Red;
    std::cout << Terminal::ApplyForegroundColor(currentDirectoryPathColor, currentDirectoryPath);
}

static void PrintGitBranch(GitRepository& repository)
{
    std::string branch          = repository.GetBranch();
    Color       delimitersColor = Color::Green;
    if (branch != "")
    {
        std::cout << Terminal::ApplyForegroundColor(delimitersColor, CHEVRONS_LEFT_DELIMITER) << branch
                  << Terminal::ApplyForegroundColor(delimitersColor, CHEVRONS_RIGHT_DELIMITER);
    }
}

static void PrintCurrentDirectoryOwnership()
{
    std::string ownershipSymbol      = " ";
    Color       ownershipSymbolColor = Color::Magenta;
    if (User::OwnsCurrentDirectory())
    {
        std::cout << Terminal::ApplyForegroundColor(ownershipSymbolColor, ownershipSymbol);
    }
}

int main()
{
    SystemTime    systemTime             = SystemTime();
    GitRepository repository             = GitRepository();
    std::string   arrowSymbol            = "⤐  ";
    std::string   cursorSymbol           = " ⩺ ";
    Color         chevronDelimitersColor = Color::Yellow;
    Color         arrowSymbolColor       = Color::Yellow;
    Color         cursorSymbolColor      = Color::Magenta;
    PrintCommandsSeparator();
    std::cout << Terminal::ApplyForegroundColor(chevronDelimitersColor, CHEVRONS_LEFT_DELIMITER);
    PrintLocalIPV4Address();
    PrintWidgetsSeparator();
    PrintStorageDeviceUsagePercentage();
    PrintWidgetsSeparator();
    PrintCalendar(systemTime);
    PrintWidgetsSeparator();
    PrintClock(systemTime);
    std::cout << Terminal::ApplyForegroundColor(chevronDelimitersColor, CHEVRONS_RIGHT_DELIMITER) << std::endl;
    PrintRootStatus();
    PrintExitCodeStatus();
    std::cout << Terminal::ApplyForegroundColor(arrowSymbolColor, arrowSymbol);
    PrintVirtualEnvironment();
    PrintCurrentDirectoryPath(repository);
    PrintGitBranch(repository);
    PrintCurrentDirectoryOwnership();
    std::cout << Terminal::ApplyForegroundColor(cursorSymbolColor, cursorSymbol) << std::endl;
    return EXIT_SUCCESS;
}
