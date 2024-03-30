#define _GNU_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef BATTERY
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

static int countDigits(int number);
static void findGitRoot(char* pwd, char** root, size_t* length);
static size_t findLastSlash(char* path, size_t length);
static void writeBatteryModule(void);
static void writeCalendarModule(void);
static void writeClockModule(void);
static void writeCommandsSeparator(void);
static void writeDirectoryAccessModule(void);
static void writeDiskModule(void);
static void writeExitCodeModule(void);
static void writeGitBranchModule(char* root, size_t length);
static void writeIPModule(void);
static void writeModulesSeparator(void);
static void writePathModule(char* pwd, char* gitRoot, size_t gitRootLength);
static void writeRootUserModule(void);
static void writeVirtualEnvModule(void);
