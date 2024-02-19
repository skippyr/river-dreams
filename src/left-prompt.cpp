#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#if !defined(BATTERY)
#define BATTERY "/sys/class/power_supply/BAT0"
#endif
#define IS_ORDINAL_DAY(ordinal_m) !((date->tm_mday - ordinal_m) % 10)

static int CountDigits(int number);
static std::string FindGitRoot(std::string directory);
static void WriteBatteryCharge(void);
static void WriteCalendar(struct std::tm* date);
static void WriteClock(struct std::tm* date);
static void WriteCommandsSeparator(struct winsize& windowSize);
static void WriteDirectoryAccess(void);
static void WriteDiskUsage(void);
static void WriteExitCodeStatus(void);
static void WriteGitBranch(std::string& gitRoot);
static void WriteLocalIPV4Address(void);
static void WriteModulesSeparator(struct winsize& windowSize);
static void WritePath(std::string& pwd, std::string& gitRoot);
static void WriteRootUserStatus(void);
static void WriteVirtualEnvironment(void);

static int modulesLength_g = 41;

static int CountDigits(int number)
{
    int digits;
    for (digits = !number; number; number /= 10)
    {
        digits++;
    }
    return (digits);
}

static std::string FindGitRoot(std::string directory)
{
    std::size_t lastSlash;
    while (1)
    {
        if (!access((directory + "/.git").c_str(), F_OK))
        {
            return (directory);
        }
        if (!directory.c_str()[1])
        {
            return ("");
        }
        directory =
            !(lastSlash = directory.find_last_of('/')) ? "/" : directory.substr(0, lastSlash);
    }
}

static void WriteBatteryCharge(void)
{
    int statusFile = open(BATTERY "/status", O_RDONLY);
    int capacityFile = open(BATTERY "/capacity", O_RDONLY);
    char statusBuffer[1];
    char capacityBuffer[5];
    int charge;
    if (statusFile < 0)
    {
        return;
    }
    read(statusFile, statusBuffer, sizeof(statusBuffer));
    read(capacityFile, capacityBuffer, sizeof(capacityBuffer));
    close(statusFile);
    close(capacityFile);
    charge = std::atoi(capacityBuffer);
    std::printf("%s%s%%f%d%%%%  ", *statusBuffer == 'C' ? "%F{yellow}󱐋 " : "",
                charge <= 5    ? "%F{red}  "
                : charge <= 25 ? "%F{yellow}  "
                : charge <= 50 ? "%F{green}  "
                               : "%F{green}  ",
                charge);
    modulesLength_g += CountDigits(charge) + 6 + (*statusBuffer == 'C') * 2;
}

static void WriteCalendar(struct std::tm* date)
{
    char buffer[13];
    std::strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
    std::printf("%%F{red}󰃭 %%f%s%s  ", buffer,
                IS_ORDINAL_DAY(1)   ? "st"
                : IS_ORDINAL_DAY(2) ? "nd"
                : IS_ORDINAL_DAY(3) ? "rd"
                                    : "th");
}

static void WriteCommandsSeparator(struct winsize& windowSize)
{
    int column;
    for (column = 0; column < windowSize.ws_col; column++)
    {
        std::printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
    }
    std::printf("%%F{yellow}:«(");
}

static void WriteDirectoryAccess(void)
{
    std::printf("%%F{magenta}%s", access(".", W_OK) ? " " : "");
}

static void WriteDiskUsage(void)
{
    fsblkcnt64_t totalBytes;
    fsblkcnt64_t remainingBytes;
    int usage;
    struct statvfs64 status;
    statvfs64("/", &status);
    totalBytes = status.f_frsize * status.f_blocks;
    remainingBytes = status.f_frsize * status.f_bavail;
    usage = ((totalBytes - remainingBytes) / static_cast<float>(totalBytes)) * 100;
    std::printf("%%F{%s}󰋊 %%f%d%%%%  ",
                usage < 70   ? "green"
                : usage < 80 ? "yellow"
                             : "red",
                usage);
    modulesLength_g += CountDigits(usage);
}

static void WriteExitCodeStatus(void)
{
    std::printf("{%%(?.%%%%.%%F{red}x)%%F{yellow}}⤐  ");
}

static void WriteGitBranch(std::string& gitRoot)
{
    std::FILE* head = std::fopen((gitRoot + "/.git/HEAD").c_str(), "r");
    int character;
    int slashes = 0;
    if (!head)
    {
        return;
    }
    std::printf("%%F{yellow}:«(%%f");
    while ((character = std::fgetc(head)) != EOF && character != '\n')
    {
        if (slashes == 2)
        {
            std::putchar(character);
        }
        else if (character == '/')
        {
            slashes++;
        }
    }
    std::printf("%%F{yellow})»");
    std::fclose(head);
}

static void WriteClock(struct std::tm* date)
{
    std::printf("%s%%f%02dh%02dm",
                date->tm_hour < 6    ? "%F{cyan}󰭎 "
                : date->tm_hour < 12 ? "%F{red}󰖨 "
                : date->tm_hour < 18 ? "%F{blue} "
                                     : "%F{yellow}󰽥 ",
                date->tm_hour, date->tm_min);
}

static void WriteLocalIPV4Address(void)
{
    char buffer[16] = "127.0.0.1";
    struct ifaddrs* interfaceList;
    struct ifaddrs* interface;
    getifaddrs(&interfaceList);
    for (interface = interfaceList; interface; interface = interface->ifa_next)
    {
        if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
            interface->ifa_flags & IFF_RUNNING && !(interface->ifa_flags & IFF_LOOPBACK))
        {
            inet_ntop(AF_INET, &((struct sockaddr_in*)interface->ifa_addr)->sin_addr, buffer,
                      sizeof(buffer));
            break;
        }
    }
    freeifaddrs(interfaceList);
    std::printf("%%F{blue} %%f%s  ", buffer);
    modulesLength_g += std::strlen(buffer);
}

static void WriteModulesSeparator(struct winsize& windowSize)
{
    int column;
    std::printf("%%F{yellow})»:");
    for (column = 0; column < windowSize.ws_col - modulesLength_g; column++)
    {
        std::printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
    }
}

static void WritePath(std::string& pwd, std::string& gitRoot)
{
    !*gitRoot.c_str() || !gitRoot.c_str()[1]
        ? std::printf("%%F{red}%%~")
        : std::printf("%%F{red}@%s", pwd.substr(gitRoot.find_last_of('/')).c_str());
}

static void WriteRootUserStatus(void)
{
    std::printf("%%F{yellow}%%(#.{%%F{red}#%%F{yellow}}.)");
}

static void WriteVirtualEnvironment(void)
{
    const char* venv = std::getenv("VIRTUAL_ENV");
    std::string path;
    if (venv)
    {
        std::printf("%%f(%s) ", (path = venv).substr(path.find_last_of('/') + 1).c_str());
    }
}

int main(void)
{
    std::string pwd = std::getenv("PWD");
    std::string gitRoot = FindGitRoot(pwd);
    std::time_t epoch = std::time(NULL);
    struct std::tm* date = std::localtime(&epoch);
    struct winsize windowSize;
    ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize);
    WriteCommandsSeparator(windowSize);
    WriteLocalIPV4Address();
    WriteDiskUsage();
    WriteBatteryCharge();
    WriteCalendar(date);
    WriteClock(date);
    WriteModulesSeparator(windowSize);
    WriteRootUserStatus();
    WriteExitCodeStatus();
    WriteVirtualEnvironment();
    WritePath(pwd, gitRoot);
    WriteGitBranch(gitRoot);
    WriteDirectoryAccess();
    std::printf("%%f \n");
    return (0);
}
