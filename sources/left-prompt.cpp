#include <arpa/inet.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <dirent.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <iostream>

static void Throw_Error(std::string description)
{
    std::cerr << "river-dreams: " << description << std::endl;
    std::exit(EXIT_FAILURE);
}

static unsigned short Get_Terminal_Width()
{
    struct winsize terminal;
    if (ioctl(2, TIOCGWINSZ, &terminal) < 0)
    {
        Throw_Error("can't get terminal width.");
    }
    return terminal.ws_col;
}

static std::string Get_Local_IPV4_Address()
{
    std::string no_address = "No Address Found";
    struct ifaddrs *interfaces;
    if (getifaddrs(&interfaces))
    {
        return no_address;
    }
    for (struct ifaddrs *interface = interfaces; interface;
         interface = interface->ifa_next)
    {
        if (!interface->ifa_addr || !(interface->ifa_flags & IFF_RUNNING) ||
            interface->ifa_flags & IFF_LOOPBACK ||
            interface->ifa_addr->sa_family != AF_INET)
        {
            continue;
        }
        char buffer[16];
        if (!inet_ntop(interface->ifa_addr->sa_family,
                       &((struct sockaddr_in *)interface->ifa_addr)->sin_addr,
                       buffer, sizeof(buffer)))
        {
            continue;
        }
        freeifaddrs(interfaces);
        return buffer;
    }
    freeifaddrs(interfaces);
    return no_address;
}

static int Get_Disk_Use_Percentage()
{
    struct statvfs metadata;
    if (statvfs("/", &metadata))
    {
        Throw_Error("can't get disk metadata.");
    }
    unsigned long total_size = metadata.f_blocks * metadata.f_frsize,
                  free_size = metadata.f_bfree * metadata.f_frsize,
                  used_size = total_size - free_size;
    return ((float)used_size / total_size) * 100;
}

static struct std::tm *Get_Local_Time()
{
    time_t epoch_time = std::time(0);
    return std::localtime(&epoch_time);
}

static std::string Get_Calendar_Week_Day(struct std::tm *local_time)
{
    switch (local_time->tm_wday)
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

static std::string Get_Calendar_Month(struct std::tm *local_time)
{
    switch (local_time->tm_mon)
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

static std::string Get_Calendar_Ordinal(struct std::tm *local_time)
{
    if (!((local_time->tm_mday - 1) % 10))
    {
        return "st";
    }
    else if (!((local_time->tm_mday - 2) % 10))
    {
        return "nd";
    }
    else if (!((local_time->tm_mday - 3) % 10))
    {
        return "rd";
    }
    return "th";
}

static std::string Get_Clock_Symbol(struct std::tm *local_time)
{
    if (local_time->tm_hour < 6)
    {
        return "%F{6}󰭎 ";
    }
    else if (local_time->tm_hour < 12)
    {
        return "%F{1}󰖨 ";
    }
    else if (local_time->tm_hour < 18)
    {
        return "%F{4} ";
    }
    return "%F{3}󰽥 ";
}

static std::string Get_Parent(std::string path)
{
    return path.substr(0, path.find_last_of("/"));
}

static std::string Get_Base_Name(std::string path)
{
    return path.substr(path.find_last_of("/") + 1);
}

static std::string Format_Time(int time)
{
    return (time < 10 ? "0" : "") + std::to_string(time);
}

static std::string Get_Environment_Variable(const char *variable)
{
    const char *value = std::getenv(variable);
    if (!value)
    {
        Throw_Error("can't access environment variables.");
    }
    return value;
}

static std::string Get_Repository_Directory(std::string path)
{
    DIR *stream = opendir(path.c_str());
    if (!stream)
    {
        return "";
    }
    bool is_repository = false;
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if (!std::strcmp(entry->d_name, ".git"))
        {
            is_repository = true;
            break;
        }
    }
    closedir(stream);
    return is_repository ? path : Get_Repository_Directory(Get_Parent(path));
}

static std::string Get_Branch()
{
    std::string repository =
                    Get_Repository_Directory(Get_Environment_Variable("PWD")),
                head = repository + "/.git/HEAD";
    std::FILE *stream = std::fopen(head.c_str(), "r");
    if (!stream)
    {
        return "";
    }
    size_t character_index = 0;
    std::string branch;
    for (char character; (character = std::fgetc(stream)) != EOF &&
                         character != '\n' && character_index < 270;
         character_index++)
    {
        if (character_index < 16)
        {
            continue;
        }
        branch += character;
    }
    std::fclose(stream);
    return branch;
}

static void Print_Horizontal_Separator()
{
    std::cout << "  ";
}

static void Print_Commands_Separator()
{
    for (unsigned short column = 0; column < Get_Terminal_Width(); column++)
    {
        std::cout << (column % 2 ? "%F{1}⊼" : "%F{3}⊵");
    }
    std::cout << std::endl;
}

static void Print_Local_IPV4_Address()
{
    std::cout << "%F{4} %f%m%F{4}@%f" << Get_Local_IPV4_Address();
}

static void Print_Disk_Use_Percentage()
{
    std::cout << "%F{3}󰋊 %f" << Get_Disk_Use_Percentage() << "%%";
}

static void Print_Calendar(struct std::tm *local_time)
{
    std::cout << "%F{1}󰸗 %f(" << Get_Calendar_Week_Day(local_time) << ") "
              << Get_Calendar_Month(local_time) << " " << local_time->tm_mday
              << Get_Calendar_Ordinal(local_time);
}

static void Print_Clock(struct std::tm *local_time)
{
    std::cout << Get_Clock_Symbol(local_time) << "%f"
              << Format_Time(local_time->tm_hour) << "h"
              << Format_Time(local_time->tm_min) << "m";
}

static void Print_Root_Status()
{
    std::cout << "%(#.{%F{1}#%F{3}}.)";
}

static void Print_Exit_Code_Status()
{
    std::cout << "{%(?.≗.%F{1}⨲%F{3})}";
}

static void Print_Virtual_Environment()
{
    const char *environment = std::getenv("VIRTUAL_ENV");
    if (environment)
    {
        std::cout << "%F{5}(%f" << Get_Base_Name(environment) << "%F{5}) ";
    }
}

static void Print_Directory()
{
    std::cout << "%F{1}%1~";
}

static void Print_Branch()
{
    std::string branch = Get_Branch();
    if (branch != "")
    {
        std::cout << "%F{2}:«(%f" << branch << "%F{2})»:";
    }
}

static void Print_Directory_Ownership()
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

int main()
{
    struct std::tm *local_time = Get_Local_Time();
    Print_Commands_Separator();
    std::cout << "%F{3}:«(";
    Print_Local_IPV4_Address();
    Print_Horizontal_Separator();
    Print_Disk_Use_Percentage();
    Print_Horizontal_Separator();
    Print_Calendar(local_time);
    Print_Horizontal_Separator();
    Print_Clock(local_time);
    std::cout << "%F{3})»:" << std::endl;
    Print_Root_Status();
    Print_Exit_Code_Status();
    std::cout << "⤐  ";
    Print_Virtual_Environment();
    Print_Directory();
    Print_Branch();
    Print_Directory_Ownership();
    std::cout << " %F{6}⩺  %f" << std::endl;
    return EXIT_SUCCESS;
}
