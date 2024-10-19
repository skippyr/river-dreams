#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <tmk.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#include <ws2tcpip.h>

#include <iphlpapi.h>

#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#if tmk_IS_OPERATING_SYSTEM_MACOS
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>
#else
#include <fcntl.h>
#endif
#include <arpa/inet.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#endif

#define SOFTWARE_NAME "river-dreams"
#define SOFTWARE_VERSION "12.1.0"
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_REPOSITORY_URL "https://github.com/skippyr/river-dreams"
#define SOFTWARE_LICENSE "MIT License"
#define SOFTWARE_CREATION_YEAR 2023
#define INITIAL_LEFT_PROMPT_REFERENCE_LENGTH 41
#define INITIAL_RIGHT_PROMPT_REFERENCE_LENGTH 0
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#define KILOBYTE(value_a) (value_a * 1024)
#endif
#if tmk_IS_OPERATING_SYSTEM_LINUX
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

enum SymbolColor
{
    SymbolColor_NoColor = -1,
    SymbolColor_DarkBlack,
    SymbolColor_DarkRed,
    SymbolColor_DarkGreen,
    SymbolColor_DarkYellow,
    SymbolColor_DarkBlue,
    SymbolColor_DarkMagenta,
    SymbolColor_DarkCyan,
    SymbolColor_DarkWhite,
    SymbolColor_LightBlack,
    SymbolColor_LightRed,
    SymbolColor_LightGreen,
    SymbolColor_LightYellow,
    SymbolColor_LightBlue,
    SymbolColor_LightMagenta,
    SymbolColor_LightCyan,
    SymbolColor_LightWhite
};

struct ExecutionArguments
{
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    bool isAdministrator;
#else
    bool isPowerShell;
#endif
    bool isLeftPrompt;
    unsigned short consoleWidth;
    unsigned short referenceLength;
    int exitCode;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    struct passwd * userInfo;
#endif
};

struct BatteryInfo
{
    int charge;
    int isCharging;
};

static void processCommandLineArguments(int totalMainArguments, const char ** mainArguments, struct ExecutionArguments * executionArguments);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
int isAdministrator(void);
#endif
static int isUnassignedIp(char * ip);
static int isBridgeIp(char * ip);
static int countDigits(int number);
static int getIpAddress(char * buffer);
static int getDiskUsage(void);
static int getBatteryInfo(struct BatteryInfo * info);
static void getWeekDayName(struct tm * localTime, char * buffer);
static void getMonthName(struct tm * localTime, char * buffer);
static void getDayOrdinal(struct tm * localTime, char * buffer);
static void writeHelpPage(void);
static void writePowerShellHelpPage(void);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void writeZshHelpPage(void);
#endif
static void writeVersionPage(void);
static void writeSymbol(struct ExecutionArguments * arguments, const char * symbol, enum SymbolColor color);
static void writeCommandLineDecoratorPromptSectiontI(struct ExecutionArguments * executionArguments);
static void writeCommandLineDecoratorPromptSectionII(struct ExecutionArguments * executionArguments);
static void writeSpacerPromptSection(void);
static void writeNetworkPromptSection(struct ExecutionArguments * executionArguments);
static void writeBatteryPromptSection(struct ExecutionArguments * executionArguments);
static void writeDiskPromptSection(struct ExecutionArguments * executionArguments);
static void writeCalendarPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime);
static void writeClockPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime);
static void writeLeftPrompt(struct ExecutionArguments * executionArguments);
static void writeRightPrompt(struct ExecutionArguments * executionArguments);
static void writeError(const char * format, ...);
static void * allocateHeapMemory(size_t size);
static void terminate(int hadSuccess);

static void processCommandLineArguments(int totalMainArguments, const char ** mainArguments, struct ExecutionArguments * executionArguments)
{
    struct tmk_CommandLineArguments commandLineArguments;
    tmk_getCommandLineArguments(totalMainArguments, mainArguments, &commandLineArguments);
    if (commandLineArguments.totalArguments == 1)
    {
        writeError("no shell provided. Use --help for help instructions.");
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    int hasShell = 0;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    int isPowerShell;
#endif
    if (!strcmp(commandLineArguments.utf8Arguments[1], "powershell"))
    {
        hasShell = 1;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
        isPowerShell = 1;
#endif
    }
    else if (!strcmp(commandLineArguments.utf8Arguments[1], "zsh"))
    {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
        writeError("the \"zsh\" shell is not available on Windows. Use --help for help instructions.");
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
#else
        hasShell = 1;
        isPowerShell = 0;
#endif
    }
    for (int offset = hasShell && commandLineArguments.totalArguments > 2 ? 2 : 1; offset < commandLineArguments.totalArguments; ++offset)
    {
        if (!strcmp(commandLineArguments.utf8Arguments[offset], "--help"))
        {
            if (hasShell)
            {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
                if (isPowerShell)
                {
#endif
                    writePowerShellHelpPage();
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
                }
                else
                {
                    writeZshHelpPage();
                }
#endif
            }
            else
            {
                writeHelpPage();
            }
            tmk_freeCommandLineArguments(&commandLineArguments);
            terminate(1);
        }
        else if (!hasShell && !strcmp(commandLineArguments.utf8Arguments[offset], "--version"))
        {
            writeVersionPage();
            tmk_freeCommandLineArguments(&commandLineArguments);
            terminate(1);
        }
        else if (strlen(commandLineArguments.utf8Arguments[offset]) > 2 && commandLineArguments.utf8Arguments[offset][0] == '-' &&
                 commandLineArguments.utf8Arguments[offset][1] == '-')
        {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
            writeError("the option \"%s\" does not exists%s. Use --help for help instructions.", commandLineArguments.utf8Arguments[offset],
                       hasShell ? " for the \"powershell\" shell" : "");
#else
            writeError("the option \"%s\" does not exists%s. Use --help for help instructions.", commandLineArguments.utf8Arguments[offset],
                       hasShell ? isPowerShell ? " for the \"powershell\" shell" : " for the \"zsh\" shell" : "");
#endif
            tmk_freeCommandLineArguments(&commandLineArguments);
            terminate(0);
        }
    }
    if (!hasShell)
    {
        writeError("invalid shell \"%s\" provided. Use --help for help instructions.", commandLineArguments.utf8Arguments[1]);
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    if (commandLineArguments.totalArguments == 2)
    {
        writeError("no prompt side provided. Use --help for help instructions.");
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    if (!strcmp(commandLineArguments.utf8Arguments[2], "left"))
    {
        executionArguments->isLeftPrompt = 1;
        executionArguments->referenceLength = INITIAL_LEFT_PROMPT_REFERENCE_LENGTH;
    }
    else if (!strcmp(commandLineArguments.utf8Arguments[2], "right"))
    {
        executionArguments->isLeftPrompt = 0;
        executionArguments->referenceLength = INITIAL_RIGHT_PROMPT_REFERENCE_LENGTH;
    }
    else
    {
        writeError("invalid prompt side \"%s\" provided. Use --help for help instructions.", commandLineArguments.utf8Arguments[2]);
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    if (!((executionArguments->isPowerShell = isPowerShell)))
    {
        struct tmk_WindowDimensions windowDimensions;
        tmk_getWindowDimensions(&windowDimensions);
        executionArguments->consoleWidth = windowDimensions.totalColumns;
        tmk_freeCommandLineArguments(&commandLineArguments);
        return;
    }
#endif
    if (commandLineArguments.totalArguments == 3)
    {
        writeError("no last exit code provided. Use --help for help instructions.");
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    char * conversionStopAddress;
    long exitCode = strtol(commandLineArguments.utf8Arguments[3], &conversionStopAddress, 10);
    if (conversionStopAddress == commandLineArguments.utf8Arguments[3] || exitCode < tmk_MINIMUM_EXIT_CODE || exitCode > tmk_MAXIMUM_EXIT_CODE)
    {
        writeError("invalid exit code \"%d\" provided. Use --help for help instructions.", exitCode);
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    executionArguments->exitCode = exitCode;
    if (commandLineArguments.totalArguments == 4)
    {
        writeError("no console width provided. Use --help for help instructions.");
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    long consoleWidth = strtol(commandLineArguments.utf8Arguments[4], &conversionStopAddress, 10);
    if (conversionStopAddress == commandLineArguments.utf8Arguments[4] || consoleWidth <= 0 || consoleWidth > 634)
    {
        writeError("invalid console width \"%d\" provided. Use --help for help instructions.", consoleWidth);
        tmk_freeCommandLineArguments(&commandLineArguments);
        terminate(0);
    }
    executionArguments->consoleWidth = consoleWidth;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    executionArguments->isAdministrator = isAdministrator();
#else
    executionArguments->userInfo = getpwuid(getuid());
#endif
    tmk_freeCommandLineArguments(&commandLineArguments);
}

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
int isAdministrator(void)
{
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID administratorsGroup;
    AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup);
    BOOL isAdministrator;
    CheckTokenMembership(NULL, administratorsGroup, &isAdministrator);
    FreeSid(administratorsGroup);
    return isAdministrator;
}
#endif

static int isUnassignedIp(char * ip)
{
    return ip[0] == '0';
}

static int isBridgeIp(char * ip)
{
    size_t length = strlen(ip);
    return ip[length - 2] == '.' && ip[length - 1] == '1';
}

static int countDigits(int number)
{
    int totalDigits;
    for (totalDigits = !number; number; number /= 10)
    {
        ++totalDigits;
    }
    return totalDigits;
}

static int getIpAddress(char * buffer)
{
    int hasIp = 0;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    ULONG adaptersListSize = KILOBYTE(15);
    PIP_ADAPTER_ADDRESSES adaptersList = allocateHeapMemory(adaptersListSize);
    if (GetAdaptersAddresses(AF_INET, 0, NULL, adaptersList, &adaptersListSize) != NO_ERROR)
    {
        free(adaptersList);
        return -!hasIp;
    }
    for (PIP_ADAPTER_ADDRESSES adapter = adaptersList; !hasIp && adapter; adapter = adapter->Next)
    {
        if (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
        {
            continue;
        }
        inet_ntop(AF_INET, &((SOCKADDR_IN *)adapter->FirstUnicastAddress->Address.lpSockaddr)->sin_addr, buffer, 16);
        hasIp = !isUnassignedIp(buffer) && !isBridgeIp(buffer);
    }
    free(adaptersList);
#else
    struct ifaddrs * interfacesList;
    if (getifaddrs(&interfacesList))
    {
        return -!hasIp;
    }
    for (struct ifaddrs * interface = interfacesList; !hasIp && interface; interface = interface->ifa_next)
    {
        if (interface->ifa_addr && interface->ifa_addr->sa_family == AF_INET && !(interface->ifa_flags & IFF_LOOPBACK))
        {
            inet_ntop(AF_INET, &((struct sockaddr_in *)interface->ifa_addr)->sin_addr, buffer, 16);
            hasIp = !isUnassignedIp(buffer) && !isBridgeIp(buffer);
        }
    }
    freeifaddrs(interfacesList);
#endif
    return -!hasIp;
}

static int getDiskUsage(void)
{
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER freeBytes;
    GetDiskFreeSpaceExW(L"C:\\", NULL, &totalBytes, &freeBytes);
    return (totalBytes.QuadPart - freeBytes.QuadPart) / (float)totalBytes.QuadPart * 100;
#else
    struct statvfs status;
    statvfs("/", &status);
    unsigned long totalBytes = status.f_frsize * status.f_blocks;
    unsigned long freeBytes = status.f_frsize * status.f_bfree;
    return (totalBytes - freeBytes) / (float)totalBytes * 100;
#endif
}

static int getBatteryInfo(struct BatteryInfo * info)
{
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    SYSTEM_POWER_STATUS battery;
    if (!GetSystemPowerStatus(&battery) || battery.BatteryFlag == 128 || battery.BatteryFlag == 255)
    {
        return -1;
    }
    info->isCharging = battery.BatteryFlag == 8;
    info->charge = battery.BatteryLifePercent == 255 ? 0 : battery.BatteryLifePercent;
#elif tmk_IS_OPERATING_SYSTEM_MACOS
    CFTypeRef powerSourcesInfo = IOPSCopyPowerSourcesInfo();
    if (!powerSourcesInfo)
    {
        return -1;
    }
    CFTypeRef powerSourcesList = IOPSCopyPowerSourcesList(powerSourcesInfo);
    CFIndex totalPowerSources = CFArrayGetCount(powerSourcesList);
    if (!totalPowerSources)
    {
        CFRelease(powerSourcesList);
        CFRelease(powerSourcesInfo);
        return -1;
    }
    CFDictionaryRef battery = CFArrayGetValueAtIndex(powerSourcesList, 0);
    CFNumberRef chargeRef = CFDictionaryGetValue(battery, CFSTR(kIOPSCurrentCapacityKey));
    CFStringRef chargeStateRef = CFDictionaryGetValue(battery, CFSTR(kIOPSPowerSourceStateKey));
    CFNumberGetValue(chargeRef, kCFNumberIntType, &info->charge);
    info->isCharging = CFStringCompare(chargeStateRef, CFSTR(kIOPSACPowerValue), 0) == kCFCompareEqualTo;
    CFRelease(powerSourcesList);
    CFRelease(powerSourcesInfo);
#else
    int statusFile = open(BATTERY "/status", O_RDONLY);
    if (statusFile < 0)
    {
        return -1;
    }
    char status;
    read(statusFile, &status, sizeof(status));
    close(statusFile);
    info->isCharging = status == 'C';
    int capacityFile = open(BATTERY "/capacity", O_RDONLY);
    info->charge = 0;
    if (capacityFile > 0)
    {
        char buffer[4];
        read(capacityFile, buffer, sizeof(buffer));
        close(capacityFile);
        char * conversionStopAddress;
        long charge = strtol(buffer, &conversionStopAddress, 10);
        if (conversionStopAddress != buffer)
        {
            info->charge = charge;
        }
    }
#endif
    return 0;
}

static void getWeekDayName(struct tm * localTime, char * buffer)
{
    memcpy(buffer,
           !localTime->tm_wday       ? "Sun"
           : localTime->tm_wday == 1 ? "Mon"
           : localTime->tm_wday == 2 ? "Tue"
           : localTime->tm_wday == 3 ? "Wed"
           : localTime->tm_wday == 4 ? "Thu"
           : localTime->tm_wday == 5 ? "Fri"
                                     : "Sat",
           4);
}

static void getMonthName(struct tm * localTime, char * buffer)
{
    memcpy(buffer,
           !localTime->tm_mon        ? "Jan"
           : localTime->tm_mon == 1  ? "Feb"
           : localTime->tm_mon == 2  ? "Mar"
           : localTime->tm_mon == 3  ? "Apr"
           : localTime->tm_mon == 4  ? "May"
           : localTime->tm_mon == 5  ? "Jun"
           : localTime->tm_mon == 6  ? "Jul"
           : localTime->tm_mon == 7  ? "Aug"
           : localTime->tm_mon == 8  ? "Sep"
           : localTime->tm_mon == 9  ? "Oct"
           : localTime->tm_mon == 10 ? "Nov"
                                     : "Dec",
           4);
}

static void getDayOrdinal(struct tm * localTime, char * buffer)
{
    memcpy(buffer, !((localTime->tm_mday - 1) % 10) ? "st" : !((localTime->tm_mday - 2) % 10) ? "nd" : !((localTime->tm_mday - 3) % 10) ? "rd" : "th", 3);
}

static void writeHelpPage(void)
{
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_write("Usage:");
    tmk_resetFontWeight();
    tmk_write(" %s <", SOFTWARE_NAME);
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("SHELL");
    tmk_resetFontEffects();
    tmk_write("> [");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("ARGUMENTS");
    tmk_resetFontEffects();
    tmk_write("] [");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("OPTIONS");
    tmk_resetFontEffects();
    tmk_writeLine("]...");
    tmk_writeLine("Writes a portion of the shell theme for a specific shell.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE SHELLS");
    tmk_resetFontWeight();
    tmk_writeLine("    powershell     Targets PowerShell.");
    tmk_writeLine("    zsh            Targets ZSH.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE OPTIONS");
    tmk_resetFontWeight();
    tmk_writeLine("     --help        Shows the software help instructions.");
    tmk_writeLine("     --version     Shows the software version.");
}

static void writePowerShellHelpPage(void)
{
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_write("Usage:");
    tmk_resetFontWeight();
    tmk_write(" %s powershell <", SOFTWARE_NAME);
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("PROMPT-SIDE");
    tmk_resetFontEffects();
    tmk_write("> <");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("EXIT-CODE");
    tmk_resetFontEffects();
    tmk_write("> <");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("CONSOLE-WIDTH");
    tmk_resetFontEffects();
    tmk_writeLine(">");
    tmk_write("       [");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("OPTIONS");
    tmk_resetFontEffects();
    tmk_writeLine("]...");
    tmk_writeLine("Writes a portion of the shell theme for PowerShell.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE SIDES");
    tmk_resetFontWeight();
    tmk_writeLine("    left      Writes the left prompt.");
    tmk_writeLine("    right     Writes the right prompt.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE OPTIONS");
    tmk_resetFontWeight();
    tmk_writeLine("    --help     Shows the shell help instructions.");
}

#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void writeZshHelpPage(void)
{
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_write("Usage:");
    tmk_resetFontWeight();
    tmk_write(" %s zsh <", SOFTWARE_NAME);
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("PROMPT-SIDE");
    tmk_resetFontEffects();
    tmk_write("> <");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("EXIT-CODE");
    tmk_resetFontEffects();
    tmk_write("> <");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("CONSOLE-WIDTH");
    tmk_resetFontEffects();
    tmk_write("> [");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write("OPTIONS");
    tmk_resetFontEffects();
    tmk_writeLine("]...");
    tmk_writeLine("Writes a portion of the shell theme for ZSH.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE SIDES");
    tmk_resetFontWeight();
    tmk_writeLine("    left      Writes the left prompt.");
    tmk_writeLine("    right     Writes the right prompt.");
    tmk_writeLine("");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeLine("AVAILABLE OPTIONS");
    tmk_resetFontWeight();
    tmk_writeLine("    --help     Shows the shell help instructions.");
}
#endif

static void writeVersionPage(void)
{
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_write(SOFTWARE_NAME);
    tmk_resetFontWeight();
    tmk_writeLine(" %s (running on %s %s)", SOFTWARE_VERSION, tmk_OPERATING_SYSTEM, tmk_CPU_ARCHITECTURE);
    tmk_write("%s. Copyright © %d %s <", SOFTWARE_LICENSE, SOFTWARE_CREATION_YEAR, SOFTWARE_AUTHOR_NAME);
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write(SOFTWARE_AUTHOR_EMAIL);
    tmk_resetFontEffects();
    tmk_writeLine(">.");
    tmk_writeLine("");
    tmk_write("Software repository available at <");
    tmk_setFontEffects(tmk_FontEffect_Underline);
    tmk_write(SOFTWARE_REPOSITORY_URL);
    tmk_resetFontEffects();
    tmk_writeLine(">.");
}

static void writeSymbol(struct ExecutionArguments * executionArguments, const char * symbol, enum SymbolColor color)
{
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("\x1b[3%dm%s\x1b[39m", color, symbol);
#else
    tmk_write(executionArguments->isPowerShell ? "\x1b[3%dm%s\x1b[39m" : "%%F{%d}%s%%f", color, symbol);
#endif
}

static void writeCommandLineDecoratorPromptSectionI(struct ExecutionArguments * executionArguments)
{
    for (int column = 0; column < executionArguments->consoleWidth; ++column)
    {
        int isOdd = column % 2;
        writeSymbol(executionArguments, isOdd ? "v" : "≥", isOdd ? SymbolColor_DarkRed : SymbolColor_DarkYellow);
    }
    writeSymbol(executionArguments, ":«(", SymbolColor_DarkYellow);
}

static void writeNetworkPromptSection(struct ExecutionArguments * executionArguments)
{
    char ip[16];
    writeSymbol(executionArguments, " ", SymbolColor_DarkBlue);
    if (getIpAddress(ip))
    {
        const char * noIpMessage = "No Address";
        tmk_write(noIpMessage);
        executionArguments->referenceLength += strlen(noIpMessage);
        return;
    }
    tmk_write(ip);
    executionArguments->referenceLength += strlen(ip);
}

static void writeBatteryPromptSection(struct ExecutionArguments * executionArguments)
{
    struct BatteryInfo info;
    if (getBatteryInfo(&info))
    {
        return;
    }
    executionArguments->referenceLength += countDigits(info.charge);
    int isLowCharge = info.charge < 15;
    int isMediumCharge = info.charge < 50;
    tmk_write("  ");
    writeSymbol(executionArguments,
                isLowCharge      ? "󱊡"
                : isMediumCharge ? "󱊢"
                                 : "󱊣",
                isLowCharge      ? SymbolColor_DarkRed
                : isMediumCharge ? SymbolColor_DarkYellow
                                 : SymbolColor_DarkGreen);
    tmk_write("%d");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("%");
#else
    tmk_write(executionArguments->isPowerShell ? "%" : "%%");
#endif
}

static void writeDiskPromptSection(struct ExecutionArguments * executionArguments)
{
    int usage = getDiskUsage();
    executionArguments->referenceLength += countDigits(usage);
    tmk_write("  ");
    writeSymbol(executionArguments, "󰋊 ", usage < 70 ? SymbolColor_DarkGreen : usage < 90 ? SymbolColor_DarkYellow : SymbolColor_DarkRed);
    tmk_write("%d", usage);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("%");
#else
    tmk_write(executionArguments->isPowerShell ? "%" : "%%");
#endif
}

static void writeCalendarPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime)
{
    char weekDay[4];
    char month[4];
    char dayOrdinal[3];
    getWeekDayName(localTime, weekDay);
    getMonthName(localTime, month);
    getDayOrdinal(localTime, dayOrdinal);
    int isDawn = localTime->tm_hour < 6;
    int isMorning = localTime->tm_hour < 12;
    int isAfternoon = localTime->tm_hour < 18;
    tmk_write("  ");
    writeSymbol(executionArguments,
                isDawn        ? "󰭎 "
                : isMorning   ? "󰖨 "
                : isAfternoon ? " "
                              : "󰽥 ",
                isDawn        ? SymbolColor_DarkMagenta
                : isMorning   ? SymbolColor_DarkRed
                : isAfternoon ? SymbolColor_DarkBlue
                              : SymbolColor_DarkYellow);
    tmk_write("(%s) %s %02d%s", weekDay, month, localTime->tm_mday, dayOrdinal);
}

static void writeClockPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime)
{
}

static void writeLeftPrompt(struct ExecutionArguments * executionArguments)
{
    time_t epochTime = time(NULL);
    struct tm * localTime = localtime(&epochTime);
    writeCommandLineDecoratorPromptSectionI(executionArguments);
    writeNetworkPromptSection(executionArguments);
    writeBatteryPromptSection(executionArguments);
    writeDiskPromptSection(executionArguments);
    writeCalendarPromptSection(executionArguments, localTime);
    writeClockPromptSection(executionArguments, localTime);
}

static void writeRightPrompt(struct ExecutionArguments * executionArguments)
{
    tmk_writeLine("Writing right prompt...");
}

static void writeError(const char * format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    tmk_setFontAnsiColor(tmk_AnsiColor_DarkRed, tmk_Layer_Foreground);
    tmk_writeError("[ERROR]");
    tmk_resetFontColors();
    tmk_writeError(" ");
    tmk_setFontWeight(tmk_FontWeight_Bold);
    tmk_writeError("%s:", SOFTWARE_NAME);
    tmk_resetFontWeight();
    tmk_writeError(" ");
    tmk_writeErrorArgumentsLine(format, arguments);
    va_end(arguments);
}

static void * allocateHeapMemory(size_t size)
{
    void * buffer = malloc(size);
    if (buffer)
    {
        return buffer;
    }
    writeError("could not allocate %zuB of memory on the heap.", size);
    terminate(0);
    return NULL;
}

static void terminate(int hadSuccess)
{
    exit(!hadSuccess);
}

int main(int totalMainArguments, const char ** mainArguments)
{
    struct ExecutionArguments executionArguments;
    processCommandLineArguments(totalMainArguments, mainArguments, &executionArguments);
    if (executionArguments.isLeftPrompt)
    {
        writeLeftPrompt(&executionArguments);
    }
    else
    {
        writeRightPrompt(&executionArguments);
    }
    return 0;
}
