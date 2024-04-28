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

static int countDigits(int number);
static char *findGitRoot(const char *pwd, size_t *length);
static size_t getLastSlashOffset(const char *path, size_t length);
static void writeBatteryModule(int *modulesLength);
static void writeCalendarModule(struct tm *localTime);
static void writeClockModule(struct tm *localTime);
static void writeCommandLineSeparator(struct winsize *terminalSize);
static void writeDirectoryAccessModule(void);
static void writeDiskModule(int *modulesLength);
static void writeExitCodeModule(void);
static void writeGitModule(const char *root, size_t length);
static void writeIPModule(int *modulesLength);
static void writeModulesSeparator(int modulesLength, struct winsize *terminalSize);
static void writePathModule(const char *pwd, const char *gitRoot, size_t gitRootLength);
static void writeRootAccessModule(void);
static void writeVenvModule(void);
static void writeZVMModule(char mode);
