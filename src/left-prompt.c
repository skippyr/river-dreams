#include "left-prompt.h"

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
#endif

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
  for (int totalSlashes = 0, character = 0;
       (character = fgetc(file)) != EOF && character != '\n';) {
    if (totalSlashes == 2) {
      tmk_write("%c", character);
    } else if (character == '/') {
      ++totalSlashes;
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
    size_t length = strlen(buffer);
    /*
     * Network interfaces that start with "0" (unnasigned) and end with
     * ".1" (brigde) must be disconsidered.
     */
    if (buffer[0] == '0' ||
        (buffer[length - 2] == '.' && buffer[length - 1] == '1')) {
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
      size_t length = strlen(buffer);
      /*
       * Network interfaces that start with "0" (unnasigned) and end with
       * ".1" (brigde) must be disconsidered.
       */
      if (buffer[0] == '0' ||
          (buffer[length - 2] == '.' && buffer[length - 1] == '1')) {
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

static void throwError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_Layer_Foreground);
  tmk_writeError("[ERROR] ");
  tmk_resetFontColors();
  tmk_writeError("river-dreams - left-prompt ");
  tmk_setFontANSIColor(tmk_ANSIColor_LightBlack, tmk_Layer_Foreground);
  tmk_writeError("(code 1)");
  tmk_resetFontColors();
  tmk_writeError(": ");
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
  exit(1);
}

static void *allocateHeapMemory(size_t size) {
  void *allocation = malloc(size);
  if (allocation) {
    return allocation;
  }
  throwError("can not allocate %zuB of memory on the heap.");
  return NULL;
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

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
int main(int totalRawCMDArguments, const char **rawCMDArguments) {
  struct tmk_CMDArguments cmdArguments;
  tmk_getCMDArguments(totalRawCMDArguments, rawCMDArguments, &cmdArguments);
  if (totalRawCMDArguments < 4) {
    tmk_freeCMDArguments(&cmdArguments);
    throwError("not enough arguments. Use it with an appropriate connector for "
               "your shell.");
  }
  int totalColumns = atoi(cmdArguments.utf8Arguments[1]);
  int lastExitCode = atoi(cmdArguments.utf8Arguments[2]);
  int isAdministrator = atoi(cmdArguments.utf8Arguments[3]);
  tmk_freeCMDArguments(&cmdArguments);
  if (!totalColumns) {
    throwError("the terminal window dimensions are invalid.");
  }
  writeCommandLineSeparator(totalColumns);
#else
int main(void) {
  struct tmk_Dimensions windowDimensions;
  if (tmk_getWindowDimensions(&windowDimensions)) {
    throwError("could not get the terminal window dimensions.");
  }
  writeCommandLineSeparator(windowDimensions.totalColumns);
#endif
  time_t epochTime = time(NULL);
  struct tm *localTime = localtime(&epochTime);
  writeIPV4Address();
  writeBatteryStatus();
  writeDiskUse();
  writeCalendar(localTime);
  writeClock(localTime);
  size_t pwdLength;
  size_t gitRootLength;
  size_t gitRootLastSeparatorOffset;
  char *gitRoot;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  writeModulesSeparator(totalColumns);
  char *utf8PWD;
  wchar_t *utf16PWD;
  getPWDPath(&utf8PWD, &utf16PWD, &pwdLength);
  findGitRoot(utf16PWD, pwdLength, &gitRoot, &gitRootLength,
              &gitRootLastSeparatorOffset);
  writeAdministratorRole(isAdministrator);
  writeLastExitCode(lastExitCode);
  tmk_write("\033[33m⤐ ");
#else
  writeModulesSeparator(windowDimensions.totalColumns);
  char *pwd;
  getPWDPath(&pwd, &pwdLength);
  findGitRoot(pwd, pwdLength, &gitRoot, &gitRootLength,
              &gitRootLastSeparatorOffset);
  writeAdministratorRole();
  writeLastExitCode();
  tmk_write("%%F{3}⤐ ");
#endif
  writeVirtualEnv();
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  writePath(utf8PWD, gitRoot, gitRootLastSeparatorOffset);
#else
  writePath(pwd, gitRoot, gitRootLastSeparatorOffset);
#endif
  writeGitBranch(gitRoot, gitRootLength);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  tmk_writeLine("\033[39m");
#else
  tmk_writeLine("%%f ");
#endif
  if (gitRoot) {
    free(gitRoot);
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  free(utf8PWD);
  free(utf16PWD);
#else
  free(pwd);
#endif
  return 0;
}
