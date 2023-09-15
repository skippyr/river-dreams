#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <ifaddrs.h>
#include <iostream>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>

static void
throwError(std::string description)
{
    std::cerr << "river-dreams: " << description << std::endl;
    std::exit(EXIT_FAILURE);
}

static unsigned short
getTerminalWidth()
{
    struct winsize terminal;
    if (ioctl(2, TIOCGWINSZ, &terminal) < 0)
    {
        throwError("can't get terminal width.");
    }
    return terminal.ws_col;
}

static std::string
getLocalIPV4Address()
{
    std::string     noAddress = "No Address Found";
    struct ifaddrs *interfaces;
    if (getifaddrs(&interfaces))
    {
        return noAddress;
    }
    for (struct ifaddrs *interface = interfaces; interface;
         interface                 = interface->ifa_next)
    {
        if (!interface->ifa_addr || !(interface->ifa_flags & IFF_RUNNING) ||
            interface->ifa_flags & IFF_LOOPBACK ||
            interface->ifa_addr->sa_family != AF_INET)
        {
            continue;
        }
        char ip[16];
        if (!inet_ntop(interface->ifa_addr->sa_family,
                       &((struct sockaddr_in *)interface->ifa_addr)->sin_addr,
                       ip, sizeof(ip)))
        {
            continue;
        }
        freeifaddrs(interfaces);
        return ip;
    }
    freeifaddrs(interfaces);
    return noAddress;
}

static int
getDiskUsePercentage()
{
    struct statvfs metadata;
    if (statvfs("/", &metadata))
    {
        throwError("can't get disk metadata.");
    }
    unsigned long totalSize = metadata.f_blocks * metadata.f_frsize,
                  freeSize  = metadata.f_bfree * metadata.f_frsize,
                  usedSize  = totalSize - freeSize;
    return ((float)usedSize / totalSize) * 100;
}

static struct std::tm *
getLocalTime()
{
    time_t epochTime = std::time(0);
    return std::localtime(&epochTime);
}

static std::string
getCalendarWeekDay(struct std::tm *localTime)
{
    switch (localTime->tm_wday)
    {
    case 0:
        return "Sun";
    case 1:
        return "Mon";
    case 2:
        return "Tue";
    case 3:
        return "Wed";
    case 4:
        return "Thu";
    case 5:
        return "Fri";
    case 6:
        return "Sat";
    default:
        return "---";
    }
}

static std::string
getCalendarMonth(struct std::tm *localTime)
{
    switch (localTime->tm_mon)
    {
    case 0:
        return "Jan";
    case 1:
        return "Feb";
    case 2:
        return "Mar";
    case 3:
        return "Apr";
    case 4:
        return "May";
    case 5:
        return "Jun";
    case 6:
        return "Jul";
    case 7:
        return "Aug";
    case 8:
        return "Sep";
    case 9:
        return "Oct";
    case 10:
        return "Nov";
    case 11:
        return "Dec";
    default:
        return "---";
    }
}

static std::string
getCalendarOrdinal(struct std::tm *localTime)
{
    if (!((localTime->tm_mday - 1) % 10))
    {
        return "st";
    }
    else if (!((localTime->tm_mday - 2) % 10))
    {
        return "nd";
    }
    else if (!((localTime->tm_mday - 3) % 10))
    {
        return "rd";
    }
    return "th";
}

static std::string
getClockSymbol(struct std::tm *localTime)
{
    if (localTime->tm_hour < 6)
    {
        return "%F{6}󰭎 ";
    }
    else if (localTime->tm_hour < 12)
    {
        return "%F{1}󰖨 ";
    }
    else if (localTime->tm_hour < 18)
    {
        return "%F{4} ";
    }
    return "%F{3}󰽥 ";
}

static std::string
getParent(std::string path)
{
    return path.substr(0, path.find_last_of("/"));
}

static std::string
getBaseName(std::string path)
{
    return path.substr(path.find_last_of("/") + 1);
}

static std::string
formatTime(int time)
{
    return (time < 10 ? "0" : "") + std::to_string(time);
}

static std::string
getEnvironmentVariable(const char *variable)
{
    const char *value = std::getenv(variable);
    if (!value)
    {
        throwError("can't access environment variables.");
    }
    return value;
}

static std::string
getRepositoryDirectory(std::string path)
{
    DIR *stream = opendir(path.c_str());
    if (!stream)
    {
        return "";
    }
    bool isRepository = false;
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if (!std::strcmp(entry->d_name, ".git"))
        {
            isRepository = true;
            break;
        }
    }
    closedir(stream);
    return isRepository ? path : getRepositoryDirectory(getParent(path));
}

static std::string
getBranch()
{
    std::string repository =
                    getRepositoryDirectory(getEnvironmentVariable("PWD")),
                head  = repository + "/.git/HEAD";
    std::FILE *stream = std::fopen(head.c_str(), "r");
    if (!stream)
    {
        return "";
    }
    size_t      characterIndex = 0;
    std::string branch;
    for (char character; (character = std::fgetc(stream)) != EOF &&
                         character != '\n' && characterIndex < 270;
         characterIndex++)
    {
        if (characterIndex < 16)
        {
            continue;
        }
        branch += character;
    }
    std::fclose(stream);
    return branch;
}

static void
printHorizontalSeparator()
{
    std::cout << "  ";
}

static void
printCommandsSeparator()
{
    for (unsigned short column = 0; column < getTerminalWidth(); column++)
    {
        std::cout << (column % 2 ? "%F{1}⊼" : "%F{3}⊵");
    }
    std::cout << std::endl;
}

static void
printLocalIPV4Address()
{
    std::cout << "%F{4} %f%m%F{4}@%f" << getLocalIPV4Address();
}

static void
printDiskUsePercentage()
{
    std::cout << "%F{3}󰋊 %f" << getDiskUsePercentage() << "%%";
}

static void
printCalendar(struct std::tm *local_time)
{
    std::cout << "%F{1}󰸗 %f(" << getCalendarWeekDay(local_time) << ") "
              << getCalendarMonth(local_time) << " " << local_time->tm_mday
              << getCalendarOrdinal(local_time);
}

static void
printClock(struct std::tm *local_time)
{
    std::cout << getClockSymbol(local_time) << "%f"
              << formatTime(local_time->tm_hour) << "h"
              << formatTime(local_time->tm_min) << "m";
}

static void
printRootStatus()
{
    std::cout << "%(#.{%F{1}#%F{3}}.)";
}

static void
printExitCodeStatus()
{
    std::cout << "{%(?.≗.%F{1}⨲%F{3})}";
}

static void
printVirtualEnvironment()
{
    const char *environment = std::getenv("VIRTUAL_ENV");
    if (environment)
    {
        std::cout << "%F{5}(%f" << getBaseName(environment) << "%F{5}) ";
    }
}

static void
printDirectory()
{
    std::cout << "%F{1}%1~";
}

static void
printBranch()
{
    std::string branch = getBranch();
    if (branch != "")
    {
        std::cout << "%F{2}:«(%f" << branch << "%F{2})»:";
    }
}

static void
printDirectoryOwnership()
{
    struct stat metadata;
    if (stat(".", &metadata))
    {
        return;
    }
    unsigned uid = getuid();
    if (uid && metadata.st_uid != uid)
    {
        std::cout << " %F{5}";
    }
}

int
main()
{
    struct std::tm *local_time = getLocalTime();
    printCommandsSeparator();
    std::cout << "%F{3}:«(";
    printLocalIPV4Address();
    printHorizontalSeparator();
    printDiskUsePercentage();
    printHorizontalSeparator();
    printCalendar(local_time);
    printHorizontalSeparator();
    printClock(local_time);
    std::cout << "%F{3})»:" << std::endl;
    printRootStatus();
    printExitCodeStatus();
    std::cout << "⤐  ";
    printVirtualEnvironment();
    printDirectory();
    printBranch();
    printDirectoryOwnership();
    std::cout << " %F{6}⩺  %f" << std::endl;
    return EXIT_SUCCESS;
}
