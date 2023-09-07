#include <arpa/inet.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <dirent.h>
#include <ifaddrs.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "errors.h"
#include "heap-memory.h"

#define Is_Month__(month)          (local_time->tm_mon == month)
#define Is_Week_Day__(week_day)    (local_time->tm_wday == week_day)
#define Is_Day_Ordinal__(ordinal) !((local_time->tm_mday - ordinal) % 10)
#define Get_Calendar_Month__                                                   \
    (Is_Month__(0) ? "Jan" : Is_Month__(1) ? "Feb" : Is_Month__(2) ? "Mar" :   \
     Is_Month__(3) ? "Apr" : Is_Month__(4) ? "May" : Is_Month__(5) ? "Jun" :   \
     Is_Month__(6) ? "Jul" : Is_Month__(7) ? "Aug" : Is_Month__(8) ? "Sep" :   \
     Is_Month__(9) ? "Oct" : Is_Month__(10) ? "Nov" : "Dec")
#define Get_Calendar_Week_Day__                                                \
    (Is_Week_Day__(0) ? "Sun" : Is_Week_Day__(1) ? "Mon" :                     \
     Is_Week_Day__(2) ? "Tue" : Is_Week_Day__(3) ? "Wed" :                     \
     Is_Week_Day__(4) ? "Thu" : Is_Week_Day__(5) ? "Fri" : "Sat")
#define Get_Calendar_Day_Ordinal__                                             \
    (Is_Day_Ordinal__(1) ? "st" : Is_Day_Ordinal__(2) ? "nd" :                 \
     Is_Day_Ordinal__(3) ? "rd" : "th")
#define Is_Hour_Before__(reference_hour) (local_time->tm_hour < reference_hour)
#define Get_Clock_Symbol__                                                     \
    (Is_Hour_Before__(6) ? "%F{6}󰭎 " : Is_Hour_Before__(12) ? "%F{1}󰖨  " :     \
     Is_Hour_Before__(18) ? "%F{4} " : "%F{3}󰽥 ")
#define Format_Time__(time) (time < 10 ? "0" : ""), time

static char          *Get_Environment_Variable(char *variable);
static unsigned short Get_Terminal_Width(void);
static char          *Get_Local_IPV4_Address(void);
static uint8_t        Get_Disk_Use_Percentage(void);
static struct tm     *Get_Local_Time(void);
static size_t         Get_Last_Slash_Index(char *path);
static char          *Get_Base_Name(char *path);
static char          *Get_Parent_Directory(char *path);
static char          *Get_Repository_Directory(char *path,
                                               uint8_t is_to_deallocate);
static char          *Get_Branch(char *repository);
static void           Print_Commands_Separator(void);
static void           Print_Horizontal_Separator(void);
static void           Print_Local_IPV4_Address(void);
static void           Print_Disk_Use_Percentage(void);
static void           Print_Calendar(struct tm *local_time);
static void           Print_Clock(struct tm *local_time);
static void           Print_Virtual_Environment(void);
static void           Print_Branch(char *repository);
static void           Print_Directory_Ownership(void);

static char *
Get_Environment_Variable(char *variable)
{
    char *value = getenv(variable);
    if (!value)
    {
        Errors__Throw_Error("can't access environment variables.");
    }
    return (value);
}

static unsigned short
Get_Terminal_Width(void)
{
    struct winsize terminal;
    if (ioctl(2, TIOCGWINSZ, &terminal) < 0)
    {
        Errors__Throw_Error("can't get terminal size.");
    }
    return (terminal.ws_col);
}

static char *
Get_Local_IPV4_Address(void)
{
    struct ifaddrs *interfaces;
    if (getifaddrs(&interfaces))
    {
        return (0);
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
                       &(((struct sockaddr_in *)interface->ifa_addr)->sin_addr),
                       buffer, sizeof(buffer)))
        {
            continue;
        }
        freeifaddrs(interfaces);
        return (Heap_Memory__Allocate_String(buffer));
    }
    freeifaddrs(interfaces);
    return (0);
}

static uint8_t
Get_Disk_Use_Percentage(void)
{
    struct statvfs metadata;
    if (statvfs("/", &metadata))
    {
        Errors__Throw_Error("can't get disk metadata.");
    }
    unsigned long total_size = metadata.f_blocks * metadata.f_frsize,
                  free_size  = metadata.f_bfree * metadata.f_frsize,
                  used_size  = total_size - free_size;
    return (((float) used_size / total_size) * 100);
}

static struct tm *
Get_Local_Time(void)
{
    time_t system_time = time(0);
    return (localtime(&system_time));
}

static size_t
Get_Last_Slash_Index(char *path)
{
    size_t last_slash_index = 0;
    for (size_t character_index = 0; character_index < strlen(path);
         character_index++)
    {
        if (path[character_index] == '/')
        {
            last_slash_index = character_index;
        }
    }
    return (last_slash_index);
}

static char *
Get_Base_Name(char *path)
{
    return (path + Get_Last_Slash_Index(path) + 1);
}

static char *
Get_Parent_Directory(char *path)
{
    if (strlen(path) == 1)
    {
        return (0);
    }
    size_t last_slash_index = Get_Last_Slash_Index(path);
    if (!last_slash_index)
    {
        return (Heap_Memory__Allocate_String("/"));
    }
    char buffer[last_slash_index + 1];
    memcpy(buffer, path, last_slash_index);
    buffer[last_slash_index] = 0;
    return (Heap_Memory__Allocate_String(buffer));
}

static char *
Get_Repository_Directory(char *path, uint8_t is_to_deallocate)
{
    DIR *stream = opendir(path);
    if (!stream)
    {
        return (0);
    }
    uint8_t is_repository = 0;
    for (struct dirent *entry; (entry = readdir(stream));)
    {
        if (!strcmp(entry->d_name, ".git"))
        {
            is_repository = 1;
            break;
        }
    }
    closedir(stream);
    if (is_repository)
    {
        return (is_to_deallocate ? path : Heap_Memory__Allocate_String(path));
    }
    char *parent = Get_Parent_Directory(path);
    if (is_to_deallocate)
    {
        Heap_Memory__Deallocate(path);
    }
    return (parent ? Get_Repository_Directory(parent, 1) : 0);
}

static char *
Get_Branch(char *repository)
{
    if (!repository)
    {
        return (0);
    }
    char *head   = Heap_Memory__Append_Strings(repository, "/.git/HEAD");
    FILE *stream = fopen(head, "r");
    Heap_Memory__Deallocate(head);
    if (!stream)
    {
        return (0);
    }
    size_t buffer_size     = 256,
           buffer_index    = 0,
           character_index = 0;
    char buffer[buffer_size];
    for (char character; (character = fgetc(stream)) != EOF &&
                          character != '\n' && buffer_index < buffer_size - 1;)
    {
        if (character_index <= 15)
        {
            character_index++;
            continue;
        }
        buffer[buffer_index] = character;
        buffer_index++;
    }
    fclose(stream);
    buffer[buffer_index] = 0;
    return (Heap_Memory__Allocate_String(buffer));
}

static void
Print_Commands_Separator(void)
{
    for (unsigned short column = 0; column < Get_Terminal_Width(); column++)
    {
        printf("%s", column % 2 ? "%F{1}⊼" : "%F{3}⊵");
    }
}

static void
Print_Horizontal_Separator(void)
{
    printf("  ");
}

static void
Print_Local_IPV4_Address(void)
{
    char *ip = Get_Local_IPV4_Address();
    printf("%%F{4} %%f%%m%%F{4}@%%f%s", ip ? ip : "No Address Found");
    Heap_Memory__Deallocate(ip);
}

static void
Print_Disk_Use_Percentage(void)
{
    printf("%%F{3}󰋊 %%f%hu%%%%", Get_Disk_Use_Percentage());
}

static void
Print_Calendar(struct tm *local_time)
{
    printf("%%F{1}󰸗 %%f(%s) %s %i%s", Get_Calendar_Week_Day__,
           Get_Calendar_Month__, local_time->tm_mday,
           Get_Calendar_Day_Ordinal__);
}

static void
Print_Clock(struct tm *local_time)
{
    printf("%s%%f%s%ih%s%im", Get_Clock_Symbol__,
           Format_Time__(local_time->tm_hour),
           Format_Time__(local_time->tm_min));
}

static void
Print_Virtual_Environment(void)
{
    char *virtual_environment = getenv("VIRTUAL_ENV");
    if (!virtual_environment)
    {
        return;
    }
    virtual_environment = Get_Base_Name(virtual_environment);
    printf("%%F{5}(%%f%s%%F{5}) ", virtual_environment);
}

static void
Print_Branch(char *repository)
{
    char *branch = Get_Branch(repository);
    if (!branch)
    {
        return;
    }
    printf("%%F{2}:«(%%f%s%%F{2})»:", branch);
    Heap_Memory__Deallocate(branch);
}

static void
Print_Directory_Ownership(void)
{
    struct stat metadata;
    if (lstat(".", &metadata))
    {
        Errors__Throw_Error("can't stat the current directory.");
    }
    unsigned uid = getuid();
    if (uid && metadata.st_uid != uid)
    {
        printf(" %%F{5}");
    }
}

int
main(void)
{
    struct tm *local_time = Get_Local_Time();
    char      *repository =
        Get_Repository_Directory(Get_Environment_Variable("PWD"), 0);
    Print_Commands_Separator();
    printf("%%F{3}:«(");
    Print_Local_IPV4_Address();
    Print_Horizontal_Separator();
    Print_Disk_Use_Percentage();
    Print_Horizontal_Separator();
    Print_Calendar(local_time);
    Print_Horizontal_Separator();
    Print_Clock(local_time);
    printf("%%F{3})»:\n%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲%%F{3})}⤐  ");
    Print_Virtual_Environment();
    printf("%%F{1}%%1~");
    Print_Branch(repository);
    Print_Directory_Ownership();
    printf(" %%F{6}⩺  %%f\n");
    Heap_Memory__Deallocate(repository);
    return (EXIT_SUCCESS);
}
