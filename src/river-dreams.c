#include "river-dreams.h"

int modulesLength_g = 41;

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static void getPWDPath(char **utf8PWD, wchar_t **utf16PWD, size_t *length) {
  DWORD size = GetFullPathNameW(L".", 0, NULL, NULL);
  *utf16PWD = allocateHeapMemory(size * sizeof(wchar_t));
  GetFullPathNameW(L".", size, *utf16PWD, NULL);
  if (size == 3) {
    /*
     * The GetFullPathNameW resolves the root of the file system without a
     * trailing backslash. This needed to be addressed in order for the
     * algorithm that finds the git root to work.
     */
    wchar_t *buffer = allocateHeapMemory(4 * sizeof(wchar_t));
    buffer[0] = (*utf16PWD)[0];
    memcpy(buffer + 1, L":\\", 3 * sizeof(wchar_t));
    free(*utf16PWD);
    *utf16PWD = buffer;
  }
  *utf8PWD = tmk_convertUTF16ToUTF8(*utf16PWD, length);
}

static size_t findLastPathSeparator(int isWide, const void *path,
                                    size_t length) {
  for (size_t offset = length - 1; offset; --offset) {
    if ((isWide ? ((const wchar_t *)path)[offset]
                : ((const char *)path)[offset]) == '\\') {
      return offset;
    }
  }
  return 0;
}

static void findGitRoot(const wchar_t *utf16PWD, size_t pwdLength,
                        char **gitRoot, size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset) {
  *gitRootLength = pwdLength;
  wchar_t *utf16GitRoot = allocateHeapMemory((pwdLength + 6) * sizeof(wchar_t));
  memcpy(utf16GitRoot, utf16PWD, pwdLength * sizeof(wchar_t));
  while (1) {
    memcpy(utf16GitRoot + *gitRootLength, L"\\.git", 6 * sizeof(wchar_t));
    *gitRootLength += 5;
    if (GetFileAttributesW(utf16GitRoot) != INVALID_FILE_ATTRIBUTES) {
      *gitRootLength -= 5;
      utf16GitRoot[*gitRootLength] = 0;
      *gitRoot = tmk_convertUTF16ToUTF8(utf16GitRoot, NULL);
      *gitRootLastSeparatorOffset =
          findLastPathSeparator(1, utf16GitRoot, *gitRootLength);
      free(utf16GitRoot);
      return;
    }
    *gitRootLength -= 5;
    utf16GitRoot[*gitRootLength] = 0;
    size_t lastSeparatorOffset =
        findLastPathSeparator(1, utf16GitRoot, *gitRootLength);
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
}

static void writeAdministratorRole(int isAdministrator) {
  if (isAdministrator) {
    tmk_write("\033[33m{\033[31m#\033[33m}");
  }
}

static void writeLastExitCode(int lastExitCode) {
  tmk_write("\033[33m{\033[3%dm%d\033[33m}", lastExitCode ? 1 : 3,
            lastExitCode);
}
#else
static void getPWDPath(char **pwd, size_t *length) {
  *pwd = realpath(".", NULL);
  *length = strlen(*pwd);
}

static size_t findLastPathSeparator(const char *path, size_t length) {
  for (size_t offset = length - 1; offset; --offset) {
    if (path[offset] == '/') {
      return offset;
    }
  }
  return 0;
}

static void findGitRoot(const char *pwd, size_t pwdLength, char **gitRoot,
                        size_t *gitRootLength,
                        size_t *gitRootLastSeparatorOffset) {
  *gitRootLength = pwdLength;
  *gitRoot = allocateHeapMemory(*gitRootLength + 6);
  memcpy(*gitRoot, pwd, pwdLength);
  while (1) {
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
}

static void writeAdministratorRole(void) {
  tmk_write("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void writeLastExitCode(void) {
  tmk_write("%%F{3}{%%(?..%%F{1})%%?%%F{3}}");
}

static void writeZshHelp(void) {
  tmk_writeLine("Usage: %s zsh [OPTIONS]...");
  tmk_writeLine("Writes a portion of the shell theme for ZSH.");
}
#endif

static int countDigits(int number) {
  int totalDigits;
  for (totalDigits = !number; number; number /= 10) {
    ++totalDigits;
  }
  return totalDigits;
}

static bool isUnassignedIp(const char *ip) {
  return ip[0] == '0';
}

static bool isBridgeIp(const char *ip) {
  size_t length = strlen(ip);
  return ip[length - 2] == '.' && ip[length - 1] == '1';
}

static void writeVirtualEnv(void) {
  char *venv = getenv("VIRTUAL_ENV");
  if (venv) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write(" \033[39m(%s)",
              venv + findLastPathSeparator(0, venv, strlen(venv)) + 1);
#else
    tmk_write(" %%f(%s)", venv + findLastPathSeparator(venv, strlen(venv)) + 1);
#endif
  }
}

static void writePath(const char *pwd, const char *gitRoot,
                      size_t gitRootLastSeparatorOffset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  int isToShort = gitRoot && gitRootLastSeparatorOffset > 2;
  tmk_write(isToShort ? " \033[31m@%s" : " \033[31m%s",
            isToShort ? pwd + gitRootLastSeparatorOffset : pwd);
#else
  int isToShort = gitRoot && gitRootLastSeparatorOffset;
  tmk_write(isToShort ? " %%F{1}@%s" : " %%F{1}%s",
            isToShort ? pwd + gitRootLastSeparatorOffset : "%~");
#endif
}

static void writeGitBranch(const char *gitRoot, size_t gitRootLength) {
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
  tmk_write("%%F{3}:«(%%f");
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
/*
 * When the HEAD file does not contain a branch, the repository is during
 * rebase and it will contain a hash instead. In this case, it must show its
 * first 7 digits.
 */
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("\033[35mrebase\033[39m:@");
#else
    tmk_write("%%F{5}rebase%%f:@");
#endif
    for (int offset = 0, character;
         offset <= 6 && (character = fgetc(file)) != EOF; ++offset) {
      tmk_write("%c", character);
    }
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m)»");
#else
  tmk_write("%%F{3})»");
#endif
  fclose(file);
}

static void writeIPV4Address(void) {
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
    SOCKADDR_IN *sockAddrIn =
        (SOCKADDR_IN *)adapter->FirstUnicastAddress->Address.lpSockaddr;
    inet_ntop(AF_INET, &sockAddrIn->sin_addr, buffer, sizeof(buffer));
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
  tmk_write("%%F{4} %%f%s", !buffer[0] ? "No Address" : buffer);
#endif
  modulesLength_g += !buffer[0] ? 10 : strlen(buffer);
}

static void writeBatteryStatus(void) {
#if defined(USE_FAKE_BATTERY) && USE_FAKE_BATTERY
  int charge = FAKE_BATTERY_CHARGE;
  int isCharging = IS_FAKE_BATTERY_CHARGING;
#else
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  SYSTEM_POWER_STATUS battery;
  if (!GetSystemPowerStatus(&battery) ||
      battery.BatteryFlag == 128 /* No system battery */ ||
      battery.BatteryFlag == 255 /* Unknown battery status */) {
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
  /*
   * Status will be either the first letter of: Charging ('C') or
   * Discharging ('D').
   */
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
    modulesLength_g += 1;
  }
  tmk_write(" \033[39m%d%%", charge);
#else
  tmk_write("  %s",
            charge <= 15   ? "%F{1}󱊡"
            : charge <= 50 ? "%F{3}󱊢"
                           : "%F{2}󱊣",
            charge);
  if (isCharging) {
    tmk_write("%%F{3}󱐋");
    modulesLength_g += 1;
  }
  tmk_write(" %%f%d%%%%", charge);
#endif
  modulesLength_g += countDigits(charge) + 5;
}

static void writeDiskUse(void) {
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
  tmk_write("  %%F{%d}󰋊 %%f%d%%%%", use < 70 ? 2 : use < 80 ? 3 : 1, use);
#endif
  modulesLength_g += countDigits(use);
}

static void writeCalendar(struct tm *localTime) {
  tmk_write("  ");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[31m󰃭 \033[39m");
#else
  tmk_write("%%F{1}󰃭 %%f");
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

static void writeClock(struct tm *localTime) {
  tmk_write("  ");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write(localTime->tm_hour < 6    ? "\033[36m󰭎 "
            : localTime->tm_hour < 12 ? "\033[31m󰖨 "
            : localTime->tm_hour < 18 ? "\033[34m "
                                      : "\033[33m󰽥 ");
  tmk_write("\033[39m");
#else
  tmk_write(localTime->tm_hour < 6    ? "%%F{6}󰭎 "
            : localTime->tm_hour < 12 ? "%%F{1}󰖨 "
            : localTime->tm_hour < 18 ? "%%F{4} "
                                      : "%%F{3}󰽥 ");
  tmk_write("%%f");
#endif
  tmk_write("%02dh%02dm", localTime->tm_hour, localTime->tm_min);
}

static void writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeError("%s: ", SOFTWARE_NAME);
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
  exit(1);
}

static void closeSoftware(bool hasExecutionSucceded) {
  exit(!hasExecutionSucceded);
}

static void *allocateHeapMemory(size_t size) {
  void *allocation = malloc(size);
  if (allocation) {
    return allocation;
  }
  writeError("can not allocate %zuB of memory on the heap.");
  closeSoftware(false);
  return NULL;
}

static void writeSoftwareHelp(void) {
  tmk_writeLine("Usage: %s <SHELL> [ARGUMENTS]... [OPTIONS]...", SOFTWARE_NAME);
  tmk_writeLine("Writes a portion of the shell theme for a specific shell.");
  tmk_writeLine("");
  tmk_writeLine("AVAILABLE SHELLS");
  tmk_writeLine("    pwsh     Targets the PowerShell shell.");
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_writeLine("    zsh      Targets the ZSH shell.");
#endif
  tmk_writeLine("");
  tmk_writeLine("Use --help with each shell for their help instructions.");
  tmk_writeLine("");
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_writeLine("    --help        Shows the software help instructions.");
  tmk_writeLine("    --version     Shows the software version.");
}

static void writeSoftwareVersion(void) {
  tmk_writeLine("%s %s (running on %s %s)", SOFTWARE_NAME, SOFTWARE_VERSION,
                tmk_OPERATING_SYSTEM, tmk_CPU_ARCHITECTURE);
  tmk_writeLine("%s. Copyright © %d %s <%s>", SOFTWARE_LICENSE,
                SOFTWARE_CREATION_YEAR, SOFTWARE_AUTHOR_NAME,
                SOFTWARE_AUTHOR_EMAIL);
  tmk_writeLine("");
  tmk_writeLine("Software repository available at <%s>",
                SOFTWARE_REPOSITORY_URL);
}

static void writePowerShellHelp(void) {
  tmk_writeLine("Usage: %s pwsh <CONSOLE-WIDTH> <LAST-EXIT-CODE> "
                "<IS-ADMINISTRATOR> [OPTIONS]...");
  tmk_writeLine("Writes a portion of the shell theme for PowerShell.");
}

static void processArguments(int totalRawCmdArguments,
                             const char **rawCmdArguments,
                             struct RuntimeInfo *runtimeInfo) {
  struct tmk_CmdArguments cmdArguments;
  tmk_getCmdArguments(totalRawCmdArguments, rawCmdArguments, &cmdArguments);
  if (cmdArguments.totalArguments == 1) {
    tmk_freeCmdArguments(&cmdArguments);
    writeError("no shell provided. Use --help for help instructions.");
    closeSoftware(false);
  }
  bool hasShell = false;
  bool isPowerShell;
  if (!strcmp(cmdArguments.utf8Arguments[1], "pwsh")) {
    isPowerShell = true;
    hasShell = true;
  } else if (!strcmp(cmdArguments.utf8Arguments[1], "zsh")) {
    isPowerShell = false;
    hasShell = true;
  }
  for (int offset = hasShell && cmdArguments.totalArguments > 2 ? 2 : 1;
       offset < cmdArguments.totalArguments; ++offset) {
    if (!strcmp(cmdArguments.utf8Arguments[offset], "--help")) {
      tmk_freeCmdArguments(&cmdArguments);
      if (hasShell) {
        if (isPowerShell) {
          writePowerShellHelp();
        } else {
#ifdef _WIN32
          writeError("the \"zsh\" shell is not available for Windows. Use "
                     "--help for help instructions.");
          closeSoftware(false);
#else
          writeZshHelp();
#endif
        }
      } else {
        writeSoftwareHelp();
      }
      closeSoftware(true);
    } else if (!hasShell &&
               !strcmp(cmdArguments.utf8Arguments[offset], "--version")) {
      tmk_freeCmdArguments(&cmdArguments);
      writeSoftwareVersion();
      closeSoftware(false);
    } else if (strlen(cmdArguments.utf8Arguments[offset]) > 2 &&
               cmdArguments.utf8Arguments[offset][0] == '-' &&
               cmdArguments.utf8Arguments[offset][1] == '-') {
      tmk_freeCmdArguments(&cmdArguments);
      writeError("the option \"%s\" does not exists%s. Use --help for help "
                 "instructions.",
                 cmdArguments.utf8Arguments[offset],
                 hasShell ? isPowerShell ? " for the \"pwsh\" shell"
                                         : " for the \"zsh\" shell"
                          : "");
      closeSoftware(false);
    }
  }
  if (!hasShell) {
    tmk_freeCmdArguments(&cmdArguments);
    writeError(
        "invalid shell \"%s\" provided. Use --help for help instructions.",
        cmdArguments.utf8Arguments[1]);
    closeSoftware(false);
  }
  tmk_freeCmdArguments(&cmdArguments);
}

static void writeCommandLineSeparator(uint16_t totalColumns) {
  for (uint16_t offset = 0; offset < totalColumns; ++offset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write(offset % 2 ? "\033[31mv" : "\033[33m≥");
#else
    tmk_write(offset % 2 ? "%%F{1}v" : "%%F{3}≥");
#endif
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m:«(");
#else
  tmk_write("%%F{3}:«(");
#endif
}

static void writeModulesSeparator(uint16_t totalColumns) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_write("\033[33m)»:");
#else
  tmk_write("%%F{3})»:");
#endif
  for (uint16_t offset = 0; offset < totalColumns - modulesLength_g; ++offset) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write(offset % 2 ? "\033[31m-" : "\033[33m=");
#else
    tmk_write(offset % 2 ? "%%F{1}-" : "%%F{3}=");
#endif
  }
}

int main(int totalCmdArguments, const char **rawCmdArguments) {
  struct RuntimeInfo runtimeInfo;
  processArguments(totalCmdArguments, rawCmdArguments, &runtimeInfo);
  return 0;
}
