#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#ifndef BATTERY
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

static int countDigits(int number);
static void findGitRoot(const char* pwd, char** root, size_t* length);
static size_t findLastSlash(const char* path, size_t length);
static void writeBatteryModule(void);
static void writeCalendarModule(struct tm* date);
static void writeClockModule(struct tm* date);
static void writeCommandsSeparator(struct winsize* windowSize);
static void writeDirectoryAccessModule(void);
static void writeDiskModule(void);
static void writeExitCodeModule(void);
static void writeGitBranchModule(const char* root, size_t length);
static void writeIPModule(void);
static void writeModulesSeparator(struct winsize* windowSize);
static void writePathModule(const char* pwd, const char* gitRoot, size_t gitRootLength);
static void writeRootUserModule(void);
static void writeVirtualEnvModule(void);
