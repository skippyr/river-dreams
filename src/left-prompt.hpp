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

static int countDigits(int number);
static std::string findGitRoot(std::string path);
static std::string getDayOrdinal(struct std::tm *localTime);
static void writeBatteryModule();
static void writeCalendarModule(struct std::tm *localTime);
static void writeClockModule(struct std::tm *localTime);
static void writeCommandLineSeparator(struct winsize &windowSize);
static void writeCWDAccessModule();
static void writeCWDPathModule(std::string cwd, std::string &gitRoot);
static void writeDiskModule();
static void writeExitCodeModule();
static void writeGitModule(std::string &gitRoot);
static void writeIPModule();
static void writeModulesSeparator(struct winsize &windowSize);
static void writePathSplits(std::string &path);
static void writeRootAccessModule();
static void writeVirtualEnvModule();
static void writeZVMModule(char mode);
