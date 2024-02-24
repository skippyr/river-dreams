#define _GNU_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#ifndef BATTERY
#define BATTERY "/sys/class/power_supply/BAT0"
#endif
#define IS_ORDINAL_DAY(ordinal_a) !((date->tm_mday - ordinal_a) % 10)

static int count_digits(int number);
static size_t find_last_slash(const char *path, size_t length);
static void find_git_root(const char *pwd, char **root, size_t *length);
static void write_battery_charge(void);
static void write_calendar(struct tm *date);
static void write_clock(struct tm *date);
static void write_commands_separator(struct winsize *window_size);
static void write_directory_access(void);
static void write_disk_usage(void);
static void write_exit_code(void);
static void write_git_branch(const char *root, size_t length);
static void write_local_ipv4_address(void);
static void write_modules_separator(struct winsize *window_size);
static void write_path(const char *pwd, const char *git_root, size_t git_root_length);
static void write_root_user_status(void);
static void write_virtual_environment(void);

static int modules_length_g = 41;

static int count_digits(int number)
{
    int digits;
    for (digits = !number; number; number /= 10)
        digits++;
    return (digits);
}

static size_t find_last_slash(const char *path, size_t length)
{
    size_t last_slash;
    for (last_slash = length - 1; last_slash; last_slash--)
        if (path[last_slash] == '/')
            return (last_slash);
    return (0);
}

static void find_git_root(const char *pwd, char **root, size_t *length)
{
    size_t pwd_length = strlen(pwd);
    *root = malloc(pwd_length + 6);
    for (*length = 0; *length < pwd_length; ++*length)
        *(*root + *length) = pwd[*length];
    while (1) {
        *(*root + *length) = '/';
        *(*root + ++*length) = '.';
        *(*root + ++*length) = 'g';
        *(*root + ++*length) = 'i';
        *(*root + ++*length) = 't';
        *(*root + ++*length) = 0;
        if (!access(*root, F_OK)) {
            *(*root + (*length -= 5)) = 0;
            return;
        }
        *(*root + (*length -= 5)) = 0;
        if (**root == '/' && !*(*root + 1))
            break;
        *(*root + (!(*length = find_last_slash(*root, *length)) ? ++*length : *length)) =
        0;
    }
    free(*root);
    *root = NULL;
}

static void write_battery_charge(void)
{
    int capacity_file = open(BATTERY "/capacity", O_RDONLY);
    int status_file = open(BATTERY "/status", O_RDONLY);
    char capacity_buffer[5];
    char status_buffer[1];
    int charge;
    if (status_file < 0)
        return;
    read(capacity_file, capacity_buffer, sizeof(capacity_buffer));
    read(status_file, status_buffer, sizeof(status_buffer));
    close(capacity_file);
    close(status_file);
    charge = atoi(capacity_buffer);
    printf("%s%s%%f%d%%%%  ", *status_buffer == 'C' ? "%F{yellow}󱐋 " : "",
           charge <= 5      ? "%F{red}  "
           : charge <= 25   ? "%F{yellow}  "
             : charge <= 50 ? "%F{green}  "
                            : "%F{green}  ",
           charge);
    modules_length_g += count_digits(charge) + 6 + (*status_buffer == 'C') * 2;
}

static void write_git_branch(const char *root, size_t length)
{
    char *path;
    FILE *head;
    int character;
    int slashes = 0;
    if (!root)
        return;
    path = malloc(length + 11);
    sprintf(path, "%s/.git/HEAD", root);
    head = fopen(path, "r");
    free(path);
    if (!head)
        return;
    printf("%%F{3}:«(%%f");
    while ((character = fgetc(head)) != EOF && character != '\n')
        if (slashes == 2)
            putchar(character);
        else if (character == '/')
            slashes++;
    printf("%%F{3})»");
    fclose(head);
}

static void write_calendar(struct tm *date)
{
    char buffer[13];
    strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
    printf("%%F{red}󰃭 %%f%s%s  ", buffer,
           IS_ORDINAL_DAY(1)     ? "st"
           : IS_ORDINAL_DAY(2)   ? "nd"
             : IS_ORDINAL_DAY(3) ? "rd"
                                 : "th");
}

static void write_clock(struct tm *date)
{
    printf("%s%%f%02dh%02dm",
           date->tm_hour < 6      ? "%F{cyan}󰭎 "
           : date->tm_hour < 12   ? "%F{red}󰖨 "
             : date->tm_hour < 18 ? "%F{blue} "
                                  : "%F{yellow}󰽥 ",
           date->tm_hour, date->tm_min);
}

static void write_commands_separator(struct winsize *window_size)
{
    int column;
    for (column = 0; column < window_size->ws_col; column++)
        printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
    printf("%%F{yellow}:«(");
}

static void write_directory_access(void)
{
    printf(access(".", W_OK) ? "%%F{magenta} " : "");
}

static void write_disk_usage(void)
{
    fsblkcnt64_t total_bytes;
    fsblkcnt64_t available_bytes;
    int usage;
    struct statvfs64 status;
    statvfs64("/", &status);
    total_bytes = status.f_frsize * status.f_blocks;
    available_bytes = status.f_frsize * status.f_bavail;
    usage = ((total_bytes - available_bytes) / (float)total_bytes) * 100;
    printf("%%F{%s}󰋊 %%f%d%%%%  ",
           usage < 70   ? "green"
           : usage < 80 ? "yellow"
                        : "red",
           usage);
    modules_length_g += count_digits(usage);
}

static void write_exit_code(void)
{
    printf("{%%(?..%%F{1})%%?%%F{3}}⤐  ");
}

static void write_local_ipv4_address(void)
{
    char buffer[16] = "127.0.0.1";
    struct ifaddrs *interfaces_list;
    struct ifaddrs *interface;
    getifaddrs(&interfaces_list);
    for (interface = interfaces_list; interface; interface = interface->ifa_next)
        if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
            interface->ifa_flags & IFF_RUNNING &&
            !(interface->ifa_flags & IFF_LOOPBACK)) {
            inet_ntop(AF_INET, &((struct sockaddr_in *)interface->ifa_addr)->sin_addr,
                      buffer, sizeof(buffer));
            break;
        }
    freeifaddrs(interfaces_list);
    printf("%%F{blue} %%f%s  ", buffer);
    modules_length_g += strlen(buffer);
}

static void write_modules_separator(struct winsize *window_size)
{
    int column;
    printf("%%F{yellow})»:");
    for (column = 0; column < window_size->ws_col - modules_length_g; column++)
        printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
}

static void write_path(const char *pwd, const char *git_root, size_t git_root_length)
{
    !git_root || (*git_root == '/' && !git_root[1])
    ? printf("%%F{1}%%~")
    : printf("%%F{1}@/%s", pwd + find_last_slash(git_root, git_root_length) + 1);
}

static void write_root_user_status(void)
{
    printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void write_virtual_environment(void)
{
    const char *path = getenv("VIRTUAL_ENV");
    if (path)
        printf("%%f(%s) ", path + find_last_slash(path, strlen(path)) + 1);
}

int main(void)
{
    const char *pwd = getenv("PWD");
    char *git_root;
    size_t git_root_length;
    time_t epoch = time(NULL);
    struct tm *date = localtime(&epoch);
    struct winsize window_size;
    ioctl(STDERR_FILENO, TIOCGWINSZ, &window_size);
    find_git_root(pwd, &git_root, &git_root_length);
    write_commands_separator(&window_size);
    write_local_ipv4_address();
    write_disk_usage();
    write_battery_charge();
    write_calendar(date);
    write_clock(date);
    write_modules_separator(&window_size);
    write_root_user_status();
    write_exit_code();
    write_virtual_environment();
    write_path(pwd, git_root, git_root_length);
    write_git_branch(git_root, git_root_length);
    write_directory_access();
    printf(" %%f\n");
    if (git_root)
        free(git_root);
    return (0);
}
