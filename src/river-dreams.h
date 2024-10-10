#include <stdio.h>
#include <string.h>
#include <time.h>
#include <tmk.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/* WinSock2.h must be put before iphlpapi.h. */
#include <WinSock2.h>
#include <ws2tcpip.h> /* For inet_ntop declaration and IDE autocompletion.
                         It is not necessary for compilation. */
#include <iphlpapi.h>
/* Windows.h must be put after WinSock2.h. */
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#if tmk_IS_OPERATING_SYSTEM_MACOS
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>
#else
#include <fcntl.h>
#endif
#include <arpa/inet.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#endif

#define SOFTWARE_NAME "river-dreams"
#define SOFTWARE_VERSION "13.0.0"
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_REPOSITORY_URL "https://github.com/skippyr/river-dreams"
#define SOFTWARE_LICENSE "MIT License"
#define SOFTWARE_CREATION_YEAR 2023
#define PARSE_OPTION(option_a, action_a)                                       \
    if (!strcmp(cmdArguments.utf8Arguments[offset], "--" option_a)) {          \
      action_a;                                                                \
      tmk_freeCmdArguments(&cmdArguments);                                     \
      close(true);                                                             \
    }
/* Emulates a fake battery for screenshots and debugging. */
#define USE_FAKE_BATTERY 0
#if defined(USE_FAKE_BATTERY)
#define FAKE_BATTERY_CHARGE 95
#define IS_FAKE_BATTERY_CHARGING 1
#endif
#if tmk_IS_OPERATING_SYSTEM_LINUX
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

struct DirectoryStat {
  int totalDirectories;
  int totalFiles;
  int totalHiddenEntries;
  int totalTemporaryEntries;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  int totalBlocks;
  int totalCharacters;
  int totalFifos;
  int totalSockets;
  int totalSymlinks;
#endif
};

struct RuntimeInfo {
#ifdef _WIN32
  bool isUserAdministrator;
#endif
  bool isPowerShell;
  unsigned short terminalWidth;
  int promptLength;
};

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void getPWDPath(char **utf8PWD, wchar_t **utf16PWD, size_t *length);
static size_t findLastPathSeparator(int isWide, const void *path,
                                    size_t length);
static void findGitRoot(const wchar_t *utf16PWD, size_t pwdLength,
                        char **gitRoot, size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset);
static void writeAdministratorRole(int isAdministrator);
static void writeLastExitCode(int lastExitCode);
#else
static void getPWDPath(char **pwd, size_t *length);
static size_t findLastPathSeparator(const char *path, size_t length);
static void findGitRoot(const char *pwd, size_t pwdLength, char **gitRoot,
                        size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset);
static void writeAdministratorRole(void);
static void writeLastExitCode(void);
#endif
static int countDigits(int number);
static bool isUnassignedIp(const char *ip);
static bool isBridgeIp(const char *ip);
static void getDirectoryStat(struct DirectoryStat *stat);
static void writeEntryStat(int isFirst, const char *symbol, int color,
                           int total);
static void writeDirectoryStat(void);
static void writeVirtualEnv(void);
static void writePath(const char *pwd, const char *gitRoot,
                      size_t gitRootLastSeparatorOffset);
static void writeGitBranch(const char *gitRoot, size_t gitRootLength);
static void writeIPV4Address(void);
static void writeBatteryStatus(void);
static void writeDiskUse(void);
static void writeCalendar(struct tm *localTime);
static void writeClock(struct tm *localTime);
static void writeCommandLineSeparator(uint16_t totalColumns);
static void writeModulesSeparator(uint16_t totalColumns);
static void writeError(const char *format, ...);
static void closeSoftware(bool hasExecutionSucceded);
static void *allocateHeapMemory(size_t size);
static void writeHelp(void);
static void writeVersion(void);
static void processArguments(int totalRawCmdArguments,
                             const char **rawCmdArguments,
                             struct RuntimeInfo *runtimeInfo);
