#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

static int countDigits(int number);
static std::string findGitRoot(std::string path);
static void writeBatteryModule();
static void writeCalendar(struct tm *date);
static void writeClock(struct tm *date);
static void writeCommandsSeparator(struct winsize &windowSize);
static void writeDirectoryAccessModule();
static void writeDiskModule();
static void writeExitCodeModule();
static void writeGitBranchModule(std::string &gitRoot);
static void writeIPModule();
static void writeModulesSeparator(struct winsize &windowSize);
static void writePath(std::string &pwd, std::string &gitRoot);
static void writeRootUserModule();
static void writeVirtualEnvModule();
