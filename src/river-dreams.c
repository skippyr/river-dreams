#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <tmk.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#include <ws2tcpip.h>

#include <iphlpapi.h>

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
#include <pwd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#endif

#define SOFTWARE_NAME "river-dreams"
#define SOFTWARE_VERSION "12.1.0"
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_REPOSITORY_URL "https://github.com/skippyr/river-dreams"
#define SOFTWARE_LICENSE "MIT License"
#define SOFTWARE_CREATION_YEAR 2023
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#define MINIMUM_EXIT_CODE -2147483647
#define MAXIMUM_EXIT_CODE 2147483647
#else
#define MINIMUM_EXIT_CODE 0
#define MAXIMUM_EXIT_CODE 255
#endif
#define USE_DEBUG_BATTERY false
#define DEBUG_BATTERY_CHARGE 25
#define IS_DEBUG_BATTERY_CHARGING true
#if tmk_IS_OPERATING_SYSTEM_LINUX
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

struct ExecutionInfo {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  bool isUserAdministrator;
#else
  bool isTargetingPowerShell;
#endif
  bool isToWriteLeftPrompt;
  unsigned short totalColumns;
  unsigned short referenceLength;
  int exitCode;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  struct passwd *userInfo;
#endif
};

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

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void getPwdPath(char **utf8Path, wchar_t **utf16Path,
                       size_t *utf8PathLength);
#else
static void getPwdPath(char **path, size_t *length);
#endif
static void *allocateHeapMemory(size_t size);
static int countDigits(int number);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static size_t findLastPathSeparator(const void *path, size_t length,
                                    bool usesWideCharacters);
#else
static size_t findLastPathSeparator(const char *path, size_t length);
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void findGitRoot(const wchar_t *utf16PwdPath, size_t utf8PwdLength,
                        char **gitRoot, size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset);
#else
static void findGitRoot(const char *pwd, size_t pwdLength, char **gitRoot,
                        size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset);
#endif
static bool isUnassignedIp(const char *ip);
static bool isBridgeIp(const char *ip);
static void writeSoftwareHelp(void);
static void writeSoftwareVersion(void);
static void writePowerShellHelp(void);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void writeZshHelp(void);
#endif
static void writeCmdSeparatorModuleI(struct ExecutionInfo *executionInfo);
static void writeCmdSeparatorModuleII(struct ExecutionInfo *executionInfo);
static void writeIpAddressModule(struct ExecutionInfo *executionInfo);
static void writeBatteryPercentageModule(struct ExecutionInfo *executionInfo);
static void writeDiskUsageModule(struct ExecutionInfo *executionInfo);
static void writeCalendarModule(struct ExecutionInfo *executionInfo,
                                struct tm *localTime);
static void writeClockModule(struct ExecutionInfo *executionInfo,
                             struct tm *localTime);
static void writeAdministratorModule(struct ExecutionInfo *executionInfo);
static void writeExitCodeModule(struct ExecutionInfo *executionInfo);
static void writeVirtualEnvModule(struct ExecutionInfo *executionInfo);
static void writePathModule(struct ExecutionInfo *executionInfo,
                            const char *pwd, const char *gitRoot,
                            size_t gitRootLastSeparatorOffset);
static void writeGitBranch(struct ExecutionInfo *executionInfo,
                           const char *gitRoot, size_t gitRootLength);
static void writeLeftPrompt(struct ExecutionInfo *executionInfo);
static void getDirectoryStat(struct DirectoryStat *directoryStat);
static void writeEntryStat(struct ExecutionInfo *executionInfo,
                           const char *symbol, int color, int total);
static void writeDirectoryStatModule(struct ExecutionInfo *executionInfo);
static void writeRightPrompt(struct ExecutionInfo *executionInfo);
static void processCmdArguments(int totalRawCmdArguments,
                                const char **rawCmdArguments,
                                struct ExecutionInfo *executionInfo);
static void writeErrorMessage(const char *format, ...);
static void closeSoftware(bool executionSucceded);

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void getPwdPath(char **utf8Path, wchar_t **utf16Path,
                       size_t *utf8PathLength) {
  DWORD utf16PathSize = GetFullPathNameW(L".", 0, NULL, NULL);
  *utf16Path = allocateHeapMemory(utf16PathSize * sizeof(wchar_t));
  GetFullPathNameW(L".", utf16PathSize, *utf16Path, NULL);
  if (utf16PathSize == 3) {
    wchar_t *buffer = allocateHeapMemory(4 * sizeof(wchar_t));
    buffer[0] = (*utf16Path)[0];
    memcpy(buffer + 1, L":\\", 3 * sizeof(wchar_t));
    free(*utf16Path);
    *utf16Path = buffer;
  }
  *utf8Path = tmk_convertUtf16ToUtf8(*utf16Path, utf8PathLength);
}
#else
static void getPwdPath(char **path, size_t *length) {
  *path = realpath(".", NULL);
  *length = strlen(*path);
}
#endif

static void *allocateHeapMemory(size_t size) {
  void *allocation = malloc(size);
  if (allocation) {
    return allocation;
  }
  writeErrorMessage("could not allocate %zuB of memory on the heap.", size);
  closeSoftware(false);
  return NULL;
}

static int countDigits(int number) {
  int totalDigits;
  for (totalDigits = !number; number; number /= 10) {
    ++totalDigits;
  }
  return totalDigits;
}

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static size_t findLastPathSeparator(const void *path, size_t length,
                                    bool usesWideCharacters) {
#else
static size_t findLastPathSeparator(const char *path, size_t length) {
#endif
  for (size_t offset = length - 1; offset; --offset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    if ((usesWideCharacters ? ((const wchar_t *)path)[offset]
                            : ((const char *)path)[offset]) == '\\') {
#else
    if (path[offset] == '/') {
#endif
      return offset;
    }
  }
  return 0;
}

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void findGitRoot(const wchar_t *utf16PwdPath, size_t utf8PwdLength,
                        char **gitRoot, size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset) {
  *gitRootLength = utf8PwdLength;
  wchar_t *utf16GitRoot =
      allocateHeapMemory((utf8PwdLength + 6) * sizeof(wchar_t));
  memcpy(utf16GitRoot, utf16PwdPath, utf8PwdLength * sizeof(wchar_t));
  while (true) {
    memcpy(utf16GitRoot + *gitRootLength, L"\\.git", 6 * sizeof(wchar_t));
    *gitRootLength += 5;
    if (GetFileAttributesW(utf16GitRoot) != INVALID_FILE_ATTRIBUTES) {
      *gitRootLength -= 5;
      utf16GitRoot[*gitRootLength] = 0;
      *gitRoot = tmk_convertUtf16ToUtf8(utf16GitRoot, NULL);
      *gitRootLastSeparatorOffset =
          findLastPathSeparator(utf16GitRoot, *gitRootLength, true);
      free(utf16GitRoot);
      return;
    }
    *gitRootLength -= 5;
    utf16GitRoot[*gitRootLength] = 0;
    size_t lastSeparatorOffset =
        findLastPathSeparator(utf16GitRoot, *gitRootLength, true);
    if (lastSeparatorOffset == 2) {
      if (*gitRootLength == 3) {
        break;
      }
      *gitRootLength = 3;
      *gitRootLastSeparatorOffset = lastSeparatorOffset;
      utf16GitRoot[3] = 0;
      continue;
    }
    *gitRootLength = lastSeparatorOffset;
    utf16GitRoot[lastSeparatorOffset] = 0;
  }
  free(utf16GitRoot);
  *gitRoot = NULL;
#else
static void findGitRoot(const char *pwd, size_t pwdLength, char **gitRoot,
                        size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset) {
  *gitRootLength = pwdLength;
  *gitRoot = allocateHeapMemory(*gitRootLength + 6);
  memcpy(*gitRoot, pwd, pwdLength);
  while (true) {
    memcpy(*gitRoot + *gitRootLength, "/.git", 6);
    *gitRootLength += 5;
    if (!access(*gitRoot, F_OK)) {
      *gitRootLength -= 5;
      (*gitRoot)[*gitRootLength] = 0;
      *gitRootLastSeparatorOffset =
          findLastPathSeparator(*gitRoot, *gitRootLength);
      return;
    }
    *gitRootLength -= 5;
    (*gitRoot)[*gitRootLength] = 0;
    size_t lastSeparatorOffset =
        findLastPathSeparator(*gitRoot, *gitRootLength);
    if (!lastSeparatorOffset) {
      if (*gitRootLength == 1) {
        break;
      }
      *gitRootLength = 1;
      (*gitRoot)[1] = 0;
      *gitRootLastSeparatorOffset = lastSeparatorOffset;
      continue;
    }
    *gitRootLength = lastSeparatorOffset;
    (*gitRoot)[lastSeparatorOffset] = 0;
  }
  free(*gitRoot);
  *gitRoot = NULL;
#endif
}

static bool isUnassignedIp(const char *ip) {
  return ip[0] == '0';
}

static bool isBridgeIp(const char *ip) {
  size_t length = strlen(ip);
  return ip[length - 2] == '.' && ip[length - 1] == '1';
}

static void writeSoftwareHelp(void) {
  tmk_write("Usage: ");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("%s ", SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_write("<");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("SHELL");
  tmk_resetFontEffects();
  tmk_write("> [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("ARGUMENTS");
  tmk_resetFontEffects();
  tmk_write("]...");
  tmk_write(" [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Writes a portion of the shell theme for a specific shell.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE SHELLS");
  tmk_resetFontWeight();
  tmk_writeLine("    pwsh     Targets the PowerShell shell.");
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_writeLine("    zsh      Targets the ZSH shell.");
#endif
  tmk_writeLine("");
  tmk_writeLine("Use --help with each shell for their help instructions.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("    --help        Shows the software help instructions.");
  tmk_writeLine("    --version     Shows the software version.");
}

static void writeSoftwareVersion(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("%s ", SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_writeLine("%s (running on %s %s)", SOFTWARE_VERSION, tmk_OPERATING_SYSTEM,
                tmk_CPU_ARCHITECTURE);
  tmk_write("%s. Copyright © %d %s <", SOFTWARE_LICENSE, SOFTWARE_CREATION_YEAR,
            SOFTWARE_AUTHOR_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("%s", SOFTWARE_AUTHOR_EMAIL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
  tmk_writeLine("");
  tmk_write("Software repository available at <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("%s", SOFTWARE_REPOSITORY_URL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
}

static void writePowerShellHelp(void) {
  tmk_write("Usage: ");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("%s", SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_write(" pwsh <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("PROMPT-SIDE");
  tmk_resetFontEffects();
  tmk_write("> <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("EXIT-CODE");
  tmk_resetFontEffects();
  tmk_write("> <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("CONSOLE-WIDTH");
  tmk_resetFontEffects();
  tmk_write("> [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Writes a portion of the shell theme for PowerShell.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE PROMPT SIDES");
  tmk_resetFontWeight();
  tmk_writeLine("    left      Writes the left prompt.");
  tmk_writeLine("    right     Writes the right prompt.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("    --help     Shows the shell help instructions.");
}

#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void writeZshHelp(void) {
  tmk_write("Usage: ");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("%s", SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_write(" zsh <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("PROMPT-SIDE");
  tmk_resetFontEffects();
  tmk_write("> [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Writes a portion of the shell theme for ZSH.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE PROMPT SIDES");
  tmk_resetFontWeight();
  tmk_writeLine("    left      Writes the left prompt.");
  tmk_writeLine("    right     Writes the right prompt.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("    --help     Shows the shell help instructions.");
}
#endif

static void writeCmdSeparatorModuleI(struct ExecutionInfo *executionInfo) {
  for (int column = 0; column < executionInfo->totalColumns; ++column) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    if (executionInfo->isTargetingPowerShell) {
#endif
      tmk_write(column % 2 ? "\033[31mv" : "\033[33m≥");
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    } else {
      tmk_write(column % 2 ? "%%F{1}v" : "%%F{3}≥");
    }
#endif
  }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (executionInfo->isTargetingPowerShell) {
#endif
    tmk_write("\033[33m:«(");
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  } else {
    tmk_write("%%F{3}:«(");
  }
#endif
}

static void writeCmdSeparatorModuleII(struct ExecutionInfo *executionInfo) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m)»:");
#else
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[33m)»:" : "%%F{3})»:");
#endif
  for (int offset = 0;
       offset < executionInfo->totalColumns - executionInfo->referenceLength;
       ++offset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write(offset % 2 ? "\033[31m-" : "\033[33m=");
#else
    tmk_write(offset % 2 ? executionInfo->isTargetingPowerShell ? "\033[31m-"
                                                                : "%%F{1}-"
              : executionInfo->isTargetingPowerShell ? "\033[33m="
                                                     : "%%F{3}=");
#endif
  }
}

static void writeIpAddressModule(struct ExecutionInfo *executionInfo) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  ULONG adaptersListSize = 15360;
  PIP_ADAPTER_ADDRESSES adaptersList = allocateHeapMemory(adaptersListSize);
  char buffer[16] = {0};
  if (GetAdaptersAddresses(AF_INET, 0, NULL, adaptersList, &adaptersListSize) !=
      NO_ERROR) {
    goto write_l;
  }
  for (PIP_ADAPTER_ADDRESSES adapter = adaptersList; !buffer[0] && adapter;
       adapter = adapter->Next) {
    if (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
      continue;
    }
    inet_ntop(AF_INET,
              &((SOCKADDR_IN *)adapter->FirstUnicastAddress->Address.lpSockaddr)
                   ->sin_addr,
              buffer, sizeof(buffer));
    if (isUnassignedIp(buffer) || isBridgeIp(buffer)) {
      buffer[0] = 0;
    }
  }
write_l:
  free(adaptersList);
  tmk_write("\033[34m \033[39m%s", !buffer[0] ? "No Address" : buffer);
#else
  struct ifaddrs *interfacesList;
  char buffer[16] = {0};
  if (getifaddrs(&interfacesList)) {
    goto write_l;
  }
  for (struct ifaddrs *interface = interfacesList; !buffer[0] && interface;
       interface = interface->ifa_next) {
    if (interface->ifa_addr && interface->ifa_addr->sa_family == AF_INET &&
        !(interface->ifa_flags & IFF_LOOPBACK)) {
      inet_ntop(AF_INET, &((struct sockaddr_in *)interface->ifa_addr)->sin_addr,
                buffer, sizeof(buffer));
      if (isUnassignedIp(buffer) || isBridgeIp(buffer)) {
        buffer[0] = 0;
      }
    }
  }
  freeifaddrs(interfacesList);
write_l:
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[34m \033[39m%s"
                                                 : "%%F{4} %%f%s",
            !buffer[0] ? "No Address" : buffer);
#endif
  executionInfo->referenceLength += !buffer[0] ? 10 : strlen(buffer);
}

static void writeBatteryPercentageModule(struct ExecutionInfo *executionInfo) {
#if USE_DEBUG_BATTERY
  int charge = DEBUG_BATTERY_CHARGE;
  int isCharging = IS_DEBUG_BATTERY_CHARGING;
#else
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  SYSTEM_POWER_STATUS battery;
  if (!GetSystemPowerStatus(&battery) || battery.BatteryFlag == 128 ||
      battery.BatteryFlag == 255) {
    return;
  }
  int isCharging = battery.BatteryFlag == 8;
  int charge =
      battery.BatteryLifePercent == 255 ? 0 : battery.BatteryLifePercent;
#elif tmk_IS_OPERATING_SYSTEM_MACOS
  CFTypeRef powerSourcesInfo = IOPSCopyPowerSourcesInfo();
  if (!powerSourcesInfo) {
    return;
  }
  CFTypeRef powerSourcesList = IOPSCopyPowerSourcesList(powerSourcesInfo);
  CFIndex totalPowerSources = CFArrayGetCount(powerSourcesList);
  if (!totalPowerSources) {
    CFRelease(powerSourcesList);
    CFRelease(powerSourcesInfo);
    return;
  }
  CFDictionaryRef battery = CFArrayGetValueAtIndex(powerSourcesList, 0);
  CFNumberRef chargeRef =
      CFDictionaryGetValue(battery, CFSTR(kIOPSCurrentCapacityKey));
  CFStringRef chargeStateRef =
      CFDictionaryGetValue(battery, CFSTR(kIOPSPowerSourceStateKey));
  int charge;
  CFNumberGetValue(chargeRef, kCFNumberIntType, &charge);
  int isCharging = CFStringCompare(chargeStateRef, CFSTR(kIOPSACPowerValue),
                                   0) == kCFCompareEqualTo;
  CFRelease(powerSourcesList);
  CFRelease(powerSourcesInfo);
#else
  int statusFile = open(BATTERY "/status", O_RDONLY);
  if (statusFile < 0) {
    return;
  }
  char status;
  read(statusFile, &status, sizeof(status));
  close(statusFile);
  int isCharging = status == 'C';
  int capacityFile = open(BATTERY "/capacity", O_RDONLY);
  int charge = 0;
  if (capacityFile > 0) {
    char buffer[4];
    read(capacityFile, buffer, sizeof(buffer));
    close(capacityFile);
    charge = atoi(buffer);
  }
#endif
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("  %s",
            charge <= 15   ? "\033[31m󱊡"
            : charge <= 50 ? "\033[33m󱊢"
                           : "\033[32m󱊣",
            charge);
  if (isCharging) {
    tmk_write("\033[33m󱐋");
    executionInfo->referenceLength += 1;
  }
  tmk_write(" \033[39m%d%%", charge);
#else
  tmk_write("  %s",
            charge <= 15 ? executionInfo->isTargetingPowerShell ? "\033[31m󱊡"
                                                                : "%F{1}󱊡"
            : charge <= 50 ? executionInfo->isTargetingPowerShell
                                 ? "\033[33m󱊢"
                                 : "%F{3}󱊢"
            : executionInfo->isTargetingPowerShell ? "\033[32m󱊣"
                                                   : "%F{2}󱊣",
            charge);
  if (isCharging) {
    tmk_write(executionInfo->isTargetingPowerShell ? "\033[33m󱐋"
                                                   : "%%F{3}󱐋");
    executionInfo->referenceLength += 1;
  }
  tmk_write(executionInfo->isTargetingPowerShell ? " \033[39m%d%%"
                                                 : " %%f%d%%%%",
            charge);
#endif
  executionInfo->referenceLength += countDigits(charge) + 5;
}

static void writeDiskUsageModule(struct ExecutionInfo *executionInfo) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  ULARGE_INTEGER totalBytes;
  ULARGE_INTEGER freeBytes;
  GetDiskFreeSpaceExW(L"C:\\", NULL, &totalBytes, &freeBytes);
  int use = (totalBytes.QuadPart - freeBytes.QuadPart) /
            (float)totalBytes.QuadPart * 100;
  tmk_write("  \033[3%dm󰋊 \033[39m%d%%",
            use < 70   ? 2
            : use < 80 ? 3
                       : 1,
            use);
#else
  struct statvfs status;
  statvfs("/", &status);
  unsigned long totalBytes = status.f_frsize * status.f_blocks;
  unsigned long freeBytes = status.f_frsize * status.f_bfree;
  int use = (totalBytes - freeBytes) / (float)totalBytes * 100;
  tmk_write(executionInfo->isTargetingPowerShell
                ? "  \033[3%dm󰋊 \033[39m%d%%"
                : "  %%F{%d}󰋊 %%f%d%%%%",
            use < 70   ? 2
            : use < 80 ? 3
                       : 1,
            use);
#endif
  executionInfo->referenceLength += countDigits(use);
}

static void writeCalendarModule(struct ExecutionInfo *executionInfo,
                                struct tm *localTime) {
  tmk_write("  ");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[31m󰃭 \033[39m");
#else
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[31m󰃭 \033[39m"
                                                 : "%%F{1}󰃭 %%f");
#endif
  tmk_write("(%s) %s %02d%s",
            !localTime->tm_wday       ? "Sun"
            : localTime->tm_wday == 1 ? "Mon"
            : localTime->tm_wday == 2 ? "Tue"
            : localTime->tm_wday == 3 ? "Wed"
            : localTime->tm_wday == 4 ? "Thu"
            : localTime->tm_wday == 5 ? "Fri"
                                      : "Sat",
            !localTime->tm_mon        ? "Jan"
            : localTime->tm_mon == 1  ? "Feb"
            : localTime->tm_mon == 2  ? "Mar"
            : localTime->tm_mon == 3  ? "Apr"
            : localTime->tm_mon == 4  ? "May"
            : localTime->tm_mon == 5  ? "Jun"
            : localTime->tm_mon == 6  ? "Jul"
            : localTime->tm_mon == 7  ? "Aug"
            : localTime->tm_mon == 8  ? "Sep"
            : localTime->tm_mon == 9  ? "Oct"
            : localTime->tm_mon == 10 ? "Nov"
                                      : "Dec",
            localTime->tm_mday,
            !((localTime->tm_mday - 1) % 10)   ? "st"
            : !((localTime->tm_mday - 2) % 10) ? "nd"
            : !((localTime->tm_mday - 3) % 10) ? "rd"
                                               : "th");
}

static void writeClockModule(struct ExecutionInfo *executionInfo,
                             struct tm *localTime) {
  tmk_write("  ");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write(localTime->tm_hour < 6    ? "\033[36m󰭎 "
            : localTime->tm_hour < 12 ? "\033[31m󰖨 "
            : localTime->tm_hour < 18 ? "\033[34m "
                                      : "\033[33m󰽥 ");
  tmk_write("\033[39m");
#else
  tmk_write(localTime->tm_hour < 6    ? executionInfo->isTargetingPowerShell
                                            ? "\033[36m󰭎 "
                                            : "%%F{6}󰭎 "
            : localTime->tm_hour < 12 ? executionInfo->isTargetingPowerShell
                                            ? "\033[31m󰖨 "
                                            : "%%F{1}󰖨 "
            : localTime->tm_hour < 18 ? executionInfo->isTargetingPowerShell
                                            ? "\033[34m "
                                            : "%%F{4} "
            : executionInfo->isTargetingPowerShell ? "\033[33m󰽥 "
                                                   : "%%F{3}󰽥 ");
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[39m" : "%%f");
#endif
  tmk_write("%02dh%02dm", localTime->tm_hour, localTime->tm_min);
}

static void writeAdministratorModule(struct ExecutionInfo *executionInfo) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (executionInfo->isTargetingPowerShell) {
    if (!executionInfo->userInfo->pw_uid) {
#else
  if (executionInfo->isUserAdministrator) {
#endif
      tmk_write("\033[33m{\033[31m#\033[33m}");
    }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  } else {
    tmk_write("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
  }
#endif
}

static void writeExitCodeModule(struct ExecutionInfo *executionInfo) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (executionInfo->isTargetingPowerShell) {
#endif
    tmk_write("\033[33m{\033[3%dm%d\033[33m}⤐ ",
              executionInfo->exitCode ? 1 : 3, executionInfo->exitCode);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  } else {
    tmk_write("%%F{3}{%%(?..%%F{1})%%?%%F{3}}⤐ ");
  }
#endif
}

static void writeVirtualEnvModule(struct ExecutionInfo *executionInfo) {
  const char *venv = getenv("VIRTUAL_ENV");
  if (venv) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write(" \033[39m(%s)",
              venv + findLastPathSeparator(venv, strlen(venv), false) + 1);
#else
    tmk_write(executionInfo->isTargetingPowerShell ? " \033[39m(%s)"
                                                   : " %%f(%s)",
              venv + findLastPathSeparator(venv, strlen(venv)) + 1);
#endif
  }
}

static void writePathModule(struct ExecutionInfo *executionInfo,
                            const char *pwdPath, const char *gitRoot,
                            size_t gitRootLastSeparatorOffset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  bool isToAbbreviate = gitRoot && gitRootLastSeparatorOffset > 2;
  tmk_write(isToAbbreviate ? " \033[31m@%s" : " \033[31m%s",
            isToAbbreviate ? pwdPath + gitRootLastSeparatorOffset : pwdPath);
#else
  bool isToAbbreviate = gitRoot && gitRootLastSeparatorOffset;
  tmk_write(executionInfo->isTargetingPowerShell ? " \033[31m" : " %%F{1}");
  if (isToAbbreviate) {
    tmk_write("@%s", pwdPath + gitRootLastSeparatorOffset);
    return;
  }
  if (executionInfo->isTargetingPowerShell) {
    size_t homeLength = strlen(executionInfo->userInfo->pw_dir);
    if (!strncmp(pwdPath, executionInfo->userInfo->pw_dir, homeLength)) {
      tmk_write("~%s", pwdPath + homeLength);
    } else {
      tmk_write("%s", pwdPath);
    }
  } else {
    tmk_write("%%~");
  }
#endif
}

static void writeGitBranch(struct ExecutionInfo *executionInfo,
                           const char *gitRoot, size_t gitRootLength) {
  if (!gitRoot) {
    return;
  }
  char *path = allocateHeapMemory(gitRootLength + 12);
  sprintf(path, "%s/.git/HEAD", gitRoot);
  FILE *file = fopen(path, "r");
  free(path);
  if (!file) {
    return;
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m:«(\033[39m");
#else
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[33m:«(\033[39m"
                                                 : "%%F{3}:«(%%f");
#endif
  bool doesHeadContainsBranch = false;
  for (int offset = 0, character;
       offset <= 3 && (character = fgetc(file)) != EOF; ++offset) {
    if (offset == 3 && character == ':') {
      doesHeadContainsBranch = true;
    }
  }
  rewind(file);
  if (doesHeadContainsBranch) {
    for (int totalSlashes = 0, character;
         (character = fgetc(file)) != EOF && character != '\n';) {
      if (totalSlashes == 2) {
        tmk_write("%c", character);
      } else if (character == '/') {
        ++totalSlashes;
      }
    }
  } else {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("\033[35mrebase\033[39m:\033[36m@\033[39m");
#else
    tmk_write(executionInfo->isTargetingPowerShell
                  ? "\033[35mrebase\033[39m:\033[36m@\033[39m"
                  : "%%F{5}rebase%%f:%%F{6}@%%f");
#endif
    for (int offset = 0, character;
         offset <= 6 && (character = fgetc(file)) != EOF; ++offset) {
      tmk_write("%c", character);
    }
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m)»");
#else
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[33m)»" : "%%F{3})»");
#endif
  fclose(file);
}

static void writeLeftPrompt(struct ExecutionInfo *executionInfo) {
  time_t epochTime = time(NULL);
  struct tm *localTime = localtime(&epochTime);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  char *utf8PwdPath;
  wchar_t *utf16PwdPath;
  size_t utf8PwdPathLength;
  char *gitRoot;
  size_t gitRootLength;
  size_t gitRootLastSeparatorOffset;
  getPwdPath(&utf8PwdPath, &utf16PwdPath, &utf8PwdPathLength);
  findGitRoot(utf16PwdPath, utf8PwdPathLength, &gitRoot, &gitRootLength,
              &gitRootLastSeparatorOffset);
#else
  char *pwdPath;
  size_t pwdPathLength;
  char *gitRoot;
  size_t gitRootLength;
  size_t gitRootLastSeparatorOffset;
  getPwdPath(&pwdPath, &pwdPathLength);
  findGitRoot(pwdPath, pwdPathLength, &gitRoot, &gitRootLength,
              &gitRootLastSeparatorOffset);
#endif
  writeCmdSeparatorModuleI(executionInfo);
  writeIpAddressModule(executionInfo);
  writeBatteryPercentageModule(executionInfo);
  writeDiskUsageModule(executionInfo);
  writeCalendarModule(executionInfo, localTime);
  writeClockModule(executionInfo, localTime);
  writeCmdSeparatorModuleII(executionInfo);
  writeAdministratorModule(executionInfo);
  writeExitCodeModule(executionInfo);
  writeVirtualEnvModule(executionInfo);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  writePathModule(executionInfo, utf8PwdPath, gitRoot,
                  gitRootLastSeparatorOffset);
  tmk_write("\033[39m\n");
#else
  writePathModule(executionInfo, pwdPath, gitRoot, gitRootLastSeparatorOffset);
  writeGitBranch(executionInfo, gitRoot, gitRootLength);
  tmk_write(executionInfo->isTargetingPowerShell ? "\033[39m\n" : " %%f\n");
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  free(utf8PwdPath);
  free(utf16PwdPath);
#else
  free(pwdPath);
#endif
  if (gitRoot) {
    free(gitRoot);
  }
}

static void getDirectoryStat(struct DirectoryStat *directoryStat) {
  memset(directoryStat, 0, sizeof(struct DirectoryStat));
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  WIN32_FIND_DATAW entry;
  HANDLE directoryStream = FindFirstFileW(L".\\*", &entry);
  if (directoryStream == INVALID_HANDLE_VALUE) {
    return;
  }
  do {
    if (entry.cFileName[0] == '.' &&
        (!entry.cFileName[1] ||
         (entry.cFileName[1] == '.' && !entry.cFileName[2]))) {
      continue;
    }
    DWORD attributes = GetFileAttributesW(entry.cFileName);
    if (attributes & FILE_ATTRIBUTE_HIDDEN) {
      ++directoryStat->totalHiddenEntries;
    }
    if (attributes & FILE_ATTRIBUTE_TEMPORARY) {
      ++directoryStat->totalTemporaryEntries;
    }
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
      ++directoryStat->totalDirectories;
    } else {
      ++directoryStat->totalFiles;
    }
  } while (FindNextFileW(directoryStream, &entry));
  FindClose(directoryStream);
#else
  DIR *directoryStream = opendir(".");
  if (!directoryStream) {
    return;
  }
  for (struct dirent *entry; (entry = readdir(directoryStream));) {
    if (entry->d_name[0] == '.') {
      if (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2])) {
        continue;
      }
      ++directoryStat->totalHiddenEntries;
    }
    size_t entryNameLength = strlen(entry->d_name);
    if (entry->d_name[entryNameLength - 1] == '~') {
      ++directoryStat->totalTemporaryEntries;
    }
    struct stat status;
    bool isSymlink = entry->d_type == DT_LNK;
    if (isSymlink) {
      ++directoryStat->totalSymlinks;
    }
#if tmk_IS_OPERATING_SYSTEM_MACOS
    if (isSymlink) {
      lstat(entry->d_name, &status);
      if (status.st_flags & UF_HIDDEN) {
        ++directoryStat->totalHiddenEntries;
      }
    }
    stat(entry->d_name, &status);
    if (status.st_flags & UF_HIDDEN) {
      ++directoryStat->totalHiddenEntries;
    }
    switch (status.st_mode & S_IFMT) {
#else
    switch (isSymlink && !stat(entry->d_name, &status) ? status.st_mode & S_IFMT
                                                       : entry->d_type) {
#endif
    case S_IFREG:
    case DT_REG:
      ++directoryStat->totalFiles;
      break;
    case S_IFDIR:
    case DT_DIR:
      ++directoryStat->totalDirectories;
      break;
    case S_IFBLK:
    case DT_BLK:
      ++directoryStat->totalBlocks;
      break;
    case S_IFCHR:
    case DT_CHR:
      ++directoryStat->totalCharacters;
      break;
    case S_IFIFO:
    case DT_FIFO:
      ++directoryStat->totalFifos;
      break;
    case S_IFSOCK:
    case DT_SOCK:
      ++directoryStat->totalSockets;
      break;
    }
  }
  closedir(directoryStream);
#endif
}

static void writeEntryStat(struct ExecutionInfo *executionInfo,
                           const char *symbol, int color, int total) {
  static bool isFirst = true;
  if (!total) {
    return;
  }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (executionInfo->isTargetingPowerShell) {
#endif
    executionInfo->referenceLength += countDigits(total) + 2 + !isFirst;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  }
#endif
  if (!isFirst) {
    tmk_write(" ");
  } else {
    isFirst = false;
  }
  if (color >= 0) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("\033[3%dm%s\033[39m%d", color, symbol, total);
#else
    tmk_write(executionInfo->isTargetingPowerShell ? "\033[3%dm%s\033[39m%d"
                                                   : "%%F{%d}%s%%f%d",
              color, symbol, total);
#endif
  } else {
    tmk_write("%s%d", symbol, total);
  }
}

static void writeDirectoryStatModule(struct ExecutionInfo *executionInfo) {
  struct DirectoryStat stat;
  getDirectoryStat(&stat);
  writeEntryStat(executionInfo, " ", -1, stat.totalFiles);
  writeEntryStat(executionInfo, " ", 3, stat.totalDirectories);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  writeEntryStat(executionInfo, "󰌷 ", 4, stat.totalSymlinks);
  writeEntryStat(executionInfo, "󰇖 ", 5, stat.totalBlocks);
  writeEntryStat(executionInfo, "󱣴 ", 2, stat.totalCharacters);
  writeEntryStat(executionInfo, "󰟦 ", 4, stat.totalFifos);
  writeEntryStat(executionInfo, "󱄙 ", 6, stat.totalSockets);
#endif
  writeEntryStat(executionInfo, "󰈉 ", 1, stat.totalHiddenEntries);
  writeEntryStat(executionInfo, "󱣹 ", 5, stat.totalTemporaryEntries);
}

static void writeRightPrompt(struct ExecutionInfo *executionInfo) {
  writeDirectoryStatModule(executionInfo);
  tmk_writeLine("");
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (executionInfo->isTargetingPowerShell) {
#endif
    tmk_write("%hu", executionInfo->referenceLength);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  }
#endif
}

static void processCmdArguments(int totalRawCmdArguments,
                                const char **rawCmdArguments,
                                struct ExecutionInfo *executionInfo) {
  struct tmk_CmdArguments cmdArguments;
  tmk_getCmdArguments(totalRawCmdArguments, rawCmdArguments, &cmdArguments);
  if (cmdArguments.totalArguments == 1) {
    writeErrorMessage("no shell provided. Use --help for help instructions.");
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  bool hasShell = false;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  bool isTargetingPowerShell;
#endif
  if (!strcmp(cmdArguments.utf8Arguments[1], "zsh")) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    writeErrorMessage("the \"zsh\" shell is not available for Windows. Use "
                      "--help for help instructions.");
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
#else
    isTargetingPowerShell = false;
    hasShell = true;
#endif
  } else if (!strcmp(cmdArguments.utf8Arguments[1], "pwsh")) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    isTargetingPowerShell = true;
#endif
    hasShell = true;
  }
  for (int offset = hasShell && cmdArguments.totalArguments > 2 ? 2 : 1;
       offset < cmdArguments.totalArguments; ++offset) {
    if (!strcmp(cmdArguments.utf8Arguments[offset], "--help")) {
      if (hasShell) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
        if (isTargetingPowerShell) {
#endif
          writePowerShellHelp();
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
        } else {
          writeZshHelp();
        }
#endif
      } else {
        writeSoftwareHelp();
      }
      tmk_freeCmdArguments(&cmdArguments);
      closeSoftware(true);
    } else if (!hasShell &&
               !strcmp(cmdArguments.utf8Arguments[offset], "--version")) {
      writeSoftwareVersion();
      tmk_freeCmdArguments(&cmdArguments);
      closeSoftware(true);
    } else if (strlen(cmdArguments.utf8Arguments[offset]) > 2 &&
               cmdArguments.utf8Arguments[offset][0] == '-' &&
               cmdArguments.utf8Arguments[offset][1] == '-') {
      writeErrorMessage("the option \"%s\" does not exists%s. Use --help for "
                        "help instructions.",
                        cmdArguments.utf8Arguments[offset],
                        hasShell
                            ?
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
                            isTargetingPowerShell ?
#endif
                                                  " for the \"pwsh\" shell"
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
                                                  : " for the \"zsh\" shell"
#endif
                            : "");
      tmk_freeCmdArguments(&cmdArguments);
      closeSoftware(false);
    }
  }
  if (!hasShell) {
    writeErrorMessage(
        "invalid shell \"%s\" provided. Use --help for help instructions.",
        cmdArguments.utf8Arguments[1]);
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  if (cmdArguments.totalArguments == 2) {
    writeErrorMessage(
        "no prompt side provided. Use --help for help instructions.");
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  if (!strcmp(cmdArguments.utf8Arguments[2], "left")) {
    executionInfo->isToWriteLeftPrompt = true;
    executionInfo->referenceLength = 41;
  } else if (!strcmp(cmdArguments.utf8Arguments[2], "right")) {
    executionInfo->isToWriteLeftPrompt = false;
    executionInfo->referenceLength = 0;
  } else {
    writeErrorMessage("invalid prompt side \"%s\" provided. Use --help for "
                      "help instructions.",
                      cmdArguments.utf8Arguments[2]);
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (!((executionInfo->isTargetingPowerShell = isTargetingPowerShell))) {
    struct tmk_Dimensions dimensions;
    tmk_getWindowDimensions(&dimensions);
    executionInfo->totalColumns = dimensions.totalColumns;
    tmk_freeCmdArguments(&cmdArguments);
    return;
  }
#endif
  if (cmdArguments.totalArguments == 3) {
    writeErrorMessage(
        "no last exit code provided. Use --help for help instructions.");
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  char *conversionResult;
  long int exitCode =
      strtol(cmdArguments.utf8Arguments[3], &conversionResult, 10);
  if (conversionResult == cmdArguments.utf8Arguments[3] ||
      exitCode < MINIMUM_EXIT_CODE || exitCode > MAXIMUM_EXIT_CODE) {
    writeErrorMessage(
        "invalid exit code \"%d\" provided. Use --help for help instructions.",
        exitCode);
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  executionInfo->exitCode = exitCode;
  if (cmdArguments.totalArguments == 4) {
    writeErrorMessage(
        "no console width provided. Use --help for help instructions.");
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  long int consoleWidth =
      strtol(cmdArguments.utf8Arguments[4], &conversionResult, 10);
  if (conversionResult == cmdArguments.utf8Arguments[4] || consoleWidth <= 0 ||
      consoleWidth > 634) {
    writeErrorMessage("invalid console width \"%d\" provided. Use --help for "
                      "help instructions.",
                      consoleWidth);
    tmk_freeCmdArguments(&cmdArguments);
    closeSoftware(false);
  }
  executionInfo->totalColumns = consoleWidth;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
  PSID administratorsGroup;
  AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                           DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                           &administratorsGroup);
  BOOL isUserAdministrator;
  CheckTokenMembership(NULL, administratorsGroup, &isUserAdministrator);
  executionInfo->isUserAdministrator = isUserAdministrator;
  FreeSid(administratorsGroup);
#else
  executionInfo->userInfo = getpwuid(getuid());
#endif
  tmk_freeCmdArguments(&cmdArguments);
}

static void writeErrorMessage(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_setFontAnsiColor(tmk_AnsiColor_DarkRed, tmk_Layer_Foreground);
  tmk_writeError("[ERROR] ");
  tmk_resetFontColors();
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeError("%s ", SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_setFontAnsiColor(tmk_AnsiColor_LightBlack, tmk_Layer_Foreground);
  tmk_writeError("(code 1)");
  tmk_resetFontColors();
  tmk_writeError(": ");
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
  exit(1);
}

static void closeSoftware(bool executionSucceded) {
  exit(!executionSucceded);
}

int main(int totalRawCmdArguments, const char **rawCmdArguments) {
  struct ExecutionInfo executionInfo;
  processCmdArguments(totalRawCmdArguments, rawCmdArguments, &executionInfo);
  if (executionInfo.isToWriteLeftPrompt) {
    writeLeftPrompt(&executionInfo);
  } else {
    writeRightPrompt(&executionInfo);
  }
  return 0;
}
