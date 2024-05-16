#include <cstring>
#include <iomanip>
#include <iostream>

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#ifndef BATTERY
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

static int CountDigits(int number);
static std::string FindGitRoot(std::string path);
static std::string GetDayOrdinal(struct std::tm* localTime);
static void WriteBatteryModule();
static void WriteCalendarModule(struct std::tm* localTime);
static void WriteClockModule(struct std::tm* localTime);
static void WriteCommandLineSeparator(struct winsize& windowSize);
static void WriteCWDAccessModule();
static void WriteCWDPathModule(std::string cwd, std::string& gitRoot);
static void WriteDiskModule();
static void WriteExitCodeModule();
static void WriteGitModule(std::string& gitRoot);
static void WriteIPModule();
static void WriteModulesSeparator(struct winsize& windowSize);
static void WritePathSplits(std::string& path);
static void WriteRootAccessModule();
static void WriteVirtualEnvModule();
static void WriteZVMModule(char mode);
