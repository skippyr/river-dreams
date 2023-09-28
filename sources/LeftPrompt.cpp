#include "Console.hpp"
#include "DateTime.hpp"
#include "GitRepository.hpp"
#include "Network.hpp"
#include "Shell.hpp"
#include "StorageDevice.hpp"
#include "User.hpp"

#define CHEVRONS_LEFT_DELIMITER      ":«("
#define CHEVRONS_RIGHT_DELIMITER     ")»:"
#define CURLY_BRACES_LEFT_DELIMITER  "{"
#define CURLY_BRACES_RIGHT_DELIMITER "}"
#define PARENTHESIS_LEFT_DELIMITER   "("
#define PARENTHESIS_RIGHT_DELIMITER  ")"

using namespace RiverDreams::Connectivity;
using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem;
using namespace RiverDreams::FileSystem::VersionControl;
using namespace RiverDreams::IO;
using namespace RiverDreams::Time;
using namespace RiverDreams::Users;

static void WriteCommandsSeparator()
{
    for (unsigned short column = 0; column < Console::GetTotalOfColumns(); column++)
    {
        bool         isOdd  = column % 2;
        std::string  symbol = isOdd ? "⊼" : "⊵";
        ConsoleColor color  = isOdd ? ConsoleColor::Red : ConsoleColor::Yellow;
        Console::Write(symbol, color);
    }
    Console::WriteLine();
}

static void WriteHorizontalSeparator()
{ Console::Write("  "); }

static void WriteLocalIPV4Address()
{
    std::string  networkSymbol   = " ";
    std::string  separatorSymbol = "@";
    ConsoleColor color           = ConsoleColor::Blue;
    Console::Write(networkSymbol, color);
    Console::Write(Network::GetHostName());
    Console::Write(separatorSymbol, color);
    Console::Write(Network::GetLocalIPV4Address());
}

static void WriteStorageDeviceUsagePercentage()
{
    std::string  symbol = "󰋊 ";
    ConsoleColor color  = ConsoleColor::Yellow;
    Console::Write(symbol, color);
    Console::Write(std::to_string(StorageDevice::GetUsagePercentage()) + "%%");
}

static void WriteCalendar(DateTime& now)
{
    std::string  symbol = "󰸗 ";
    ConsoleColor color  = ConsoleColor::Red;
    Console::Write(symbol, color);
    Console::Write(now.GetCalendar());
}

static void WriteClock(DateTime& now)
{
    std::string  symbol;
    ConsoleColor color;
    switch (now.GetDayMoment())
    {
    case DayMoment::Dawn:
        symbol = "󰭎 ";
        color  = ConsoleColor::Magenta;
        break;
    case DayMoment::Morning:
        symbol = "󰖨 ";
        color  = ConsoleColor::Red;
        break;
    case DayMoment::Afternoon:
        symbol = " ";
        color  = ConsoleColor::Blue;
        break;
    case DayMoment::Night:
        symbol = "󰽥 ";
        color  = ConsoleColor::Yellow;
        break;
    default:
        symbol = " ";
        color  = ConsoleColor::Blue;
    }
    Console::Write(symbol, color);
    Console::Write(now.GetClock());
}

static void WriteRootStatus()
{
    std::string  symbol          = "#";
    ConsoleColor symbolColor     = ConsoleColor::Red;
    ConsoleColor delimitersColor = ConsoleColor::Yellow;
    if (User::IsRoot())
    {
        Console::Write(CURLY_BRACES_LEFT_DELIMITER, delimitersColor);
        Console::Write(symbol, symbolColor);
        Console::Write(CURLY_BRACES_RIGHT_DELIMITER, delimitersColor);
    }
}

static void WriteExitCodeStatus()
{
    std::string  successSymbol      = "≗";
    std::string  errorSymbol        = "⨲";
    ConsoleColor successSymbolColor = ConsoleColor::Yellow;
    ConsoleColor errorSymbolColor   = ConsoleColor::Red;
    ConsoleColor delimitersColor    = ConsoleColor::Yellow;
    Console::Write(CURLY_BRACES_LEFT_DELIMITER, delimitersColor);
    Console::Write(Shell::WrapOnErrorEvent(
        Console::ApplyForegroundColor(errorSymbol, errorSymbolColor),
        Console::ApplyForegroundColor(successSymbol, successSymbolColor)));
    Console::Write(CURLY_BRACES_RIGHT_DELIMITER, delimitersColor);
}

static void WriteVirtualEnv()
{
    std::string  virtualEnvName  = Path::GetBase(EnvironmentVariables::GetVirtualEnv());
    ConsoleColor delimitersColor = ConsoleColor::Magenta;
    if (virtualEnvName != "")
    {
        Console::Write(PARENTHESIS_LEFT_DELIMITER, delimitersColor);
        Console::Write(virtualEnvName);
        Console::Write(PARENTHESIS_RIGHT_DELIMITER, delimitersColor);
        Console::Write(" ");
    }
}

static void WritePWDAbbreviated(GitRepository& repository)
{
    ConsoleColor color = ConsoleColor::Red;
    Console::Write(Path::GetPWDAbbreviated(repository.GetRootDirectoryPath()), color);
}

static void WriteBranch(GitRepository& repository)
{
    std::string  branch          = repository.GetBranch();
    ConsoleColor delimitersColor = ConsoleColor::Green;
    if (branch != "")
    {
        Console::Write(CHEVRONS_LEFT_DELIMITER, delimitersColor);
        Console::Write(branch);
        Console::Write(CHEVRONS_RIGHT_DELIMITER, delimitersColor);
    }
}

static void WritePWDOwnership()
{
    std::string  symbol = " ";
    ConsoleColor color  = ConsoleColor::Magenta;
    if (!User::OwnsPWD())
    { Console::Write(symbol, color); }
}

int main()
{
    DateTime      now               = DateTime();
    GitRepository repository        = GitRepository();
    std::string   arrowSymbol       = "⤐  ";
    std::string   cursorSymbol      = " ⩺ ";
    ConsoleColor  arrowSymbolColor  = ConsoleColor::Yellow;
    ConsoleColor  cursorSymbolColor = ConsoleColor::Magenta;
    ConsoleColor  delimitersColor   = ConsoleColor::Yellow;
    WriteCommandsSeparator();
    Console::Write(CHEVRONS_LEFT_DELIMITER, delimitersColor);
    WriteLocalIPV4Address();
    WriteHorizontalSeparator();
    WriteStorageDeviceUsagePercentage();
    WriteHorizontalSeparator();
    WriteCalendar(now);
    WriteHorizontalSeparator();
    WriteClock(now);
    Console::WriteLine(CHEVRONS_RIGHT_DELIMITER, delimitersColor);
    WriteRootStatus();
    WriteExitCodeStatus();
    Console::Write(arrowSymbol, arrowSymbolColor);
    WriteVirtualEnv();
    WritePWDAbbreviated(repository);
    WriteBranch(repository);
    WritePWDOwnership();
    Console::WriteLine(cursorSymbol, cursorSymbolColor);
    return EXIT_SUCCESS;
}
