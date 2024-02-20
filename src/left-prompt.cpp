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

static int count_digits(int number);
static std::string find_git_root(std::string pwd);
static void write_battery_charge();
static void write_calendar(struct std::tm* date);
static void write_clock(struct std::tm* date);
static void write_commands_separator(struct winsize& window_size);
static void write_directory_access();
static void write_disk_usage();
static void write_exit_code_status();
static void write_git_branch(std::string& git_root);
static void write_local_ipv4_address();
static void write_modules_separator(struct winsize& window_size);
static void write_path(std::string& pwd, std::string& git_root);
static void write_root_user_status();
static void write_virtual_environment();

static int modules_length_g = 41;

static int count_digits(int number)
{
    int digits;
    for (digits = !number; number; number /= 10) {
        digits++;
    }
    return (digits);
}

static std::string find_git_root(std::string pwd)
{
    while (1) {
        if (!access((pwd + "/.git").c_str(), F_OK)) {
            return (pwd);
        } else if (pwd.length() == 1) {
            return ("");
        }
        std::size_t last_slash = pwd.find_last_of('/');
        pwd = !last_slash ? "/" : pwd.substr(0, last_slash);
    }
}

static void write_battery_charge()
{
    int status_file = open(BATTERY "/status", O_RDONLY);
    int capacity_file = open(BATTERY "/capacity", O_RDONLY);
    char status_buffer[1];
    char capacity_buffer[5];
    if (status_file < 0) {
        return;
    }
    read(status_file, status_buffer, sizeof(status_buffer));
    read(capacity_file, capacity_buffer, sizeof(capacity_buffer));
    close(status_file);
    close(capacity_file);
    int charge = std::atoi(capacity_buffer);
    std::printf("%s%s%%f%d%%%%  ", *status_buffer == 'C' ? "%F{yellow}󱐋 " : "",
                charge <= 5      ? "%F{red}  "
                : charge <= 25   ? "%F{yellow}  "
                  : charge <= 50 ? "%F{green}  "
                                 : "%F{green}  ",
                charge);
    modules_length_g += count_digits(charge) + 6 + (*status_buffer == 'C') * 2;
}

static void write_git_branch(std::string& git_root)
{
    std::FILE* head = std::fopen((git_root + "/.git/HEAD").c_str(), "r");
    if (!head) {
        return;
    }
    std::printf("%%F{yellow}:«(%%f");
    int slashes = 0;
    int character;
    while ((character = std::fgetc(head)) != EOF && character != '\n') {
        if (slashes == 2) {
            std::putchar(character);
        } else if (character == '/') {
            slashes++;
        }
    }
    std::printf("%%F{yellow})»");
    std::fclose(head);
}

static void write_calendar(struct std::tm* date)
{
    char buffer[13];
    std::strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
    std::printf("%%F{red}󰃭 %%f%s%s  ", buffer,
                IS_ORDINAL_DAY(1)     ? "st"
                : IS_ORDINAL_DAY(2)   ? "nd"
                  : IS_ORDINAL_DAY(3) ? "rd"
                                      : "th");
}

static void write_clock(struct std::tm* date)
{
    std::printf("%s%%f%02dh%02dm",
                date->tm_hour < 6      ? "%F{cyan}󰭎 "
                : date->tm_hour < 12   ? "%F{red}󰖨 "
                  : date->tm_hour < 18 ? "%F{blue} "
                                       : "%F{yellow}󰽥 ",
                date->tm_hour, date->tm_min);
}

static void write_commands_separator(struct winsize& window_size)
{
    for (int column = 0; column < window_size.ws_col; column++) {
        std::printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
    }
    std::printf("%%F{yellow}:«(");
}

static void write_directory_access()
{
    std::printf("%%F{magenta}%s", access(".", W_OK) ? " " : "");
}

static void write_disk_usage()
{
    struct statvfs64 status;
    statvfs64("/", &status);
    fsblkcnt64_t totalBytes = status.f_frsize * status.f_blocks;
    fsblkcnt64_t remainingBytes = status.f_frsize * status.f_bavail;
    int usage = ((totalBytes - remainingBytes) / static_cast<float>(totalBytes)) * 100;
    std::printf("%%F{%s}󰋊 %%f%d%%%%  ",
                usage < 70   ? "green"
                : usage < 80 ? "yellow"
                             : "red",
                usage);
    modules_length_g += count_digits(usage);
}

static void write_exit_code_status()
{
    std::printf("{%%(?.%%%%.%%F{red}x)%%F{yellow}}⤐  ");
}

static void write_local_ipv4_address()
{
    char buffer[16] = "127.0.0.1";
    struct ifaddrs* interfaces_list;
    struct ifaddrs* interface;
    getifaddrs(&interfaces_list);
    for (interface = interfaces_list; interface; interface = interface->ifa_next) {
        if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
            interface->ifa_flags & IFF_RUNNING && !(interface->ifa_flags & IFF_LOOPBACK)) {
            inet_ntop(AF_INET,
                      &(reinterpret_cast<struct sockaddr_in*>(interface->ifa_addr))->sin_addr,
                      buffer, sizeof(buffer));
            break;
        }
    }
    freeifaddrs(interfaces_list);
    std::printf("%%F{blue} %%f%s  ", buffer);
    modules_length_g += std::strlen(buffer);
}

static void write_modules_separator(struct winsize& window_size)
{
    std::printf("%%F{yellow})»:");
    for (int column = 0; column < window_size.ws_col - modules_length_g; column++) {
        std::printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
    }
}

static void write_path(std::string& pwd, std::string& git_root)
{
    git_root.length() < 2 ? std::printf("%%F{red}%%~")
                          : std::printf("%%F{red}@%s", pwd.c_str() + git_root.find_last_of('/'));
}

static void write_root_user_status()
{
    std::printf("%%F{yellow}%%(#.{%%F{red}#%%F{yellow}}.)");
}

static void write_virtual_environment()
{
    if (const char* venv = getenv("VIRTUAL_ENV")) {
        std::printf("%%f(%s) ", venv + std::string(venv).find_last_of('/') + 1);
    }
}

int main()
{
    std::string pwd = std::getenv("PWD");
    std::string git_root = find_git_root(pwd);
    std::time_t epoch = std::time(NULL);
    struct std::tm* date = std::localtime(&epoch);
    struct winsize window_size;
    ioctl(STDERR_FILENO, TIOCGWINSZ, &window_size);
    write_commands_separator(window_size);
    write_local_ipv4_address();
    write_disk_usage();
    write_battery_charge();
    write_calendar(date);
    write_clock(date);
    write_modules_separator(window_size);
    write_root_user_status();
    write_exit_code_status();
    write_virtual_environment();
    write_path(pwd, git_root);
    write_git_branch(git_root);
    write_directory_access();
    std::printf(" %%f\n");
    return (0);
}
