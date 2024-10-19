#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <tmk.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/* On Windows, the order of the headers may cause compilation errors and some external libraries need to be linked. */
#include <ws2tcpip.h>

#include <iphlpapi.h>

#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#if tmk_IS_OPERATING_SYSTEM_MACOS
/* On MacOS, the software needs to be linked with the IOKit and CoreFoundation frameworks. */
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

/** The name of the software. */
#define SOFTWARE_NAME "river-dreams"
/** The version of the software. */
#define SOFTWARE_VERSION "12.1.0"
/** The name of the software author. */
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
/** The e-mail of the software author. */
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
/** The URL of the software repository. */
#define SOFTWARE_REPOSITORY_URL "https://github.com/skippyr/river-dreams"
/** The license of the software. */
#define SOFTWARE_LICENSE "MIT License"
/** The year the software was created. */
#define SOFTWARE_CREATION_YEAR 2023
/** The initial value of the reference length for the left prompt. */
#define INITIAL_LEFT_PROMPT_REFERENCE_LENGTH 41
/** The initial value of the reference length for the right prompt. */
#define INITIAL_RIGHT_PROMPT_REFERENCE_LENGTH 0
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/** Converts a kilobyte value to bytes. */
#define KILOBYTE(value_a) (value_a * 1024)
#endif
#if tmk_IS_OPERATING_SYSTEM_LINUX
/** The path to the Linux file where the battery info is stored. */
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

/** Contains the available colors. */
enum Color
{
    /** No color. */
    Color_None = -1,
    /** The dark variant of the black color. */
    Color_DarkBlack,
    /** The dark variant of the red color. */
    Color_DarkRed,
    /** The dark variant of the green color. */
    Color_DarkGreen,
    /** The dark variant of the yellow color. */
    Color_DarkYellow,
    /** The dark variant of the blue color. */
    Color_DarkBlue,
    /** The dark variant of the magenta color. */
    Color_DarkMagenta,
    /** The dark variant of the cyan color. */
    Color_DarkCyan,
    /** The dark variant of the white color. */
    Color_DarkWhite,
    /** The light variant of the black color. */
    Color_LightBlack,
    /** The light variant of the red color. */
    Color_LightRed,
    /** The light variant of the green color. */
    Color_LightGreen,
    /** The light variant of the yellow color. */
    Color_LightYellow,
    /** The light variant of the blue color. */
    Color_LightBlue,
    /** The light variant of the magenta color. */
    Color_LightMagenta,
    /** The light variant of the cyan color. */
    Color_LightCyan,
    /** The light variant of the white color. */
    Color_LightWhite
};

/** Contains the arguments that dictates the software execution. */
struct ExecutionArguments
{
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    /** A boolean that states the current user is member of the Administrators group. */
    bool isAdministrator;
#else
    /** A boolean that states the PowerShell is being targeted. */
    bool isPowerShell;
#endif
    /** A boolean that states the left prompt must be written. */
    bool isLeftPrompt;
    /** The console width. */
    unsigned short consoleWidth;
    /** A reference length to be used for writing by both prompt sides. */
    unsigned short referenceLength;
    int exitCode;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    struct passwd * userInfo;
#endif
};

/** Contains info about a battery. */
struct BatteryInfo
{
    /** The current charge of the battery. */
    int charge;
    /** A boolean that states the battery is charging. */
    int isCharging;
};

/**
 * Process the command line arguments and gets the execution arguments.
 * @param totalMainArguments The total number of arguments received as the first argument by the main function.
 * @param mainArguments The arguments received as the second argument by the main function.
 * @param executionArguments The address where the execution arguments will be put into.
 */
static void processCommandLineArguments(int totalMainArguments, const char ** mainArguments, struct ExecutionArguments * executionArguments);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/**
 * Checks if the current user is member of the Administrators group.
 * @return A boolean that states the current user is member of the Administrators group.
 */
int isAdministrator(void);
#endif
/**
 * Checks if an IP address is the unassigned one: "0.0.0.0".
 * @param ip The IP to be checked.
 * @return A boolean that states the IP address is unassigned.
 */
static int isUnassignedIP(char * ip);
/**
 * Checks if an IP address is a bridge one: which ends with ".1".
 * @param ip The IP to be checked.
 * @return A boolean that states the IP is bridge.
 */
static int isBridgeIP(char * ip);
/**
 * Counts the digits in a number.
 * @param number The number to be checked.
 * @return The total digits in the number.
 */
static int countDigits(int number);
/**
 * Gets the IP address of the machine.
 * @param buffer A buffer where the IP will be stored. It must have 16 bytes.
 * @return 0 if successful or -1 otherwise.
 */
static int getIPAddress(char * buffer);
/**
 * Gets the disk usage.
 * @return The disk usage.
 */
static int getDiskUsage(void);
/**
 * Gets the battery info if possible.
 * @return 0 if successful or -1 otherwise (in case there is an error or no battery).
 */
static int getBatteryInfo(struct BatteryInfo * info);
/**
 * Gets the week day name in an abbreviated matter.
 * @param localTime The time in the local timezone.
 * @param buffer The buffer where the name will be stored. It must have 4 bytes.
 */
static void getWeekDayName(struct tm * localTime, char * buffer);
/**
 * Gets the month name in an abbreviated matter.
 * @param localTime The time in the local timezone.
 * @param buffer The buffer where the name will be stored. It must have 4 bytes.
 */
static void getMonthName(struct tm * localTime, char * buffer);
/**
 * Gets the day ordinal.
 * @param localTime The time in the local timezone.
 * @param buffer The buffer where the name will be stored. It must have 3 bytes.
 */
static void getDayOrdinal(struct tm * localTime, char * buffer);
/** Writes the software help page. */
static void writeHelpPage(void);
/** Writes the PowerShell help page. */
static void writePowerShellHelpPage(void);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
/** Writes the ZSH help page. */
static void writeZshHelpPage(void);
#endif
/** Writes the software version page. */
static void writeVersionPage(void);
/**
 * Writes a symbol of the prompt.
 * @param executionArguments The execution arguments.
 * @param symbol The symbol to be written.
 * @param color The color of the symbol.
 */
static void writeSymbol(struct ExecutionArguments * executionArguments, const char * symbol, enum Color color);
/**
 * Writes the first command line decorator prompt section.
 * @param executionArguments The execution arguments.
 */
static void writeCommandLineDecoratorPromptSectiontI(struct ExecutionArguments * executionArguments);
/**
 * Writes the second command line decorator prompt section.
 * @param executionArguments The execution arguments.
 */
static void writeCommandLineDecoratorPromptSectionII(struct ExecutionArguments * executionArguments);
static void writeNetworkPromptSection(struct ExecutionArguments * executionArguments);
/**
 * Writes the network prompt section.
 * @param executionArguments The execution arguments.
 */
static void writeBatteryPromptSection(struct ExecutionArguments * executionArguments);
/**
 * Writes the disk prompt section.
 * @param executionArguments The execution arguments.
 */
static void writeDiskPromptSection(struct ExecutionArguments * executionArguments);
/**
 * Writes the calendar prompt section.
 * @param executionArguments The execution arguments.
 * @param localTime The time in the local timezone.
 */
static void writeCalendarPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime);
/**
 * Writes the clock prompt section.
 * @param executionArguments The execution arguments.
 * @param localTime The time in the local timezone.
 */
static void writeClockPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime);
/**
 * Writes the left prompt.
 * @param executionArguments The execution arguments.
 */
static void writeLeftPrompt(struct ExecutionArguments * executionArguments);
/**
 * Writes the right prompt.
 * @param executionArguments The execution arguments.
 */
static void writeRightPrompt(struct ExecutionArguments * executionArguments);
/**
 * Formats and write an error message to the standard error stream.
 * @param format The format to be used. It accept the same format specifiers as the print function family.
 * @param ... The arguments to be formatted.
 */
static void writeError(const char * format, ...);
/**
 * Allocates a block of memory on the heap.
 * @param size The size of the block.
 * @return The address of the block allocated or NULL otherwise.
 */
static void * allocateHeapMemory(size_t size);
/**
 * Terminates the software execution.
 * @param hadSuccess A boolean that states the execution was successful.
 */
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

static int isUnassignedIP(char * ip)
{
    return ip[0] == '0';
}

static int isBridgeIP(char * ip)
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

static int getIPAddress(char * buffer)
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
        hasIp = !isUnassignedIP(buffer) && !isBridgeIP(buffer);
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
            hasIp = !isUnassignedIP(buffer) && !isBridgeIP(buffer);
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

static void writeSymbol(struct ExecutionArguments * executionArguments, const char * symbol, enum Color color)
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
        writeSymbol(executionArguments, isOdd ? "v" : "≥", isOdd ? Color_DarkRed : Color_DarkYellow);
    }
    writeSymbol(executionArguments, ":«(", Color_DarkYellow);
}

static void writeNetworkPromptSection(struct ExecutionArguments * executionArguments)
{
    char ip[16];
    writeSymbol(executionArguments, " ", Color_DarkBlue);
    if (getIPAddress(ip))
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
                isLowCharge      ? Color_DarkRed
                : isMediumCharge ? Color_DarkYellow
                                 : Color_DarkGreen);
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
    writeSymbol(executionArguments, "󰋊 ", usage < 70 ? Color_DarkGreen : usage < 90 ? Color_DarkYellow : Color_DarkRed);
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
    tmk_write("  ");
    writeSymbol(executionArguments, "󰃭 ", Color_DarkRed);
    tmk_write("(%s) %s %02d%s", weekDay, month, localTime->tm_mday, dayOrdinal);
}

static void writeClockPromptSection(struct ExecutionArguments * executionArguments, struct tm * localTime)
{
    tmk_write("  ");
    int isDawn = localTime->tm_hour < 6;
    int isMorning = localTime->tm_hour < 12;
    int isAfternoon = localTime->tm_hour < 18;
    writeSymbol(executionArguments,
                isDawn        ? "󰭎 "
                : isMorning   ? "󰖨 "
                : isAfternoon ? " "
                              : "󰽥 ",
                isDawn        ? Color_DarkMagenta
                : isMorning   ? Color_DarkRed
                : isAfternoon ? Color_DarkBlue
                              : Color_DarkYellow);
    tmk_write("%02dh%02dm", localTime->tm_hour, localTime->tm_min);
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
