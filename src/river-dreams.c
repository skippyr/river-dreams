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
#define INITIAL_LEFT_PROMPT_REFERENCE_LENGTH 41
#define INITIAL_RIGHT_PROMPT_REFERENCE_LENGTH 0
#if tmk_IS_OPERATING_SYSTEM_LINUX
#define BATTERY "/sys/class/power_supply/BAT0"
#endif

struct ExecutionArguments {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  bool isAdministrator;
#else
  bool isPowerShell;
#endif
  bool isLeftPrompt;
  unsigned short consoleWidth;
  unsigned short referenceLength;
  int exitCode;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  struct passwd *userInfo;
#endif
};

static void
processCommandLineArguments(int totalMainArguments, const char **mainArguments,
                            struct ExecutionArguments *executionArguments);
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
int isAdministrator(void);
#endif
static void writeHelpPage(void);
static void writePowerShellHelpPage(void);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void writeZshHelpPage(void);
#endif
static void writeVersionPage(void);
static void writeError(const char *format, ...);
static void terminate(int hadSuccess);

static void
processCommandLineArguments(int totalMainArguments, const char **mainArguments,
                            struct ExecutionArguments *executionArguments) {
  struct tmk_CommandLineArguments commandLineArguments;
  tmk_getCommandLineArguments(totalMainArguments, mainArguments,
                              &commandLineArguments);
  if (commandLineArguments.totalArguments == 1) {
    writeError("no shell provided. Use --help for help instructions.");
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  int hasShell = 0;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  int isPowerShell;
#endif
  if (!strcmp(commandLineArguments.utf8Arguments[1], "powershell")) {
    hasShell = 1;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
    isPowerShell = 1;
#endif
  } else if (!strcmp(commandLineArguments.utf8Arguments[1], "zsh")) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    writeError("the \"zsh\" shell is not available on Windows. Use --help for "
               "help instructions.");
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
#else
    hasShell = 1;
    isPowerShell = 0;
#endif
  }
  for (int offset = hasShell && commandLineArguments.totalArguments > 2 ? 2 : 1;
       offset < commandLineArguments.totalArguments; ++offset) {
    if (!strcmp(commandLineArguments.utf8Arguments[offset], "--help")) {
      if (hasShell) {
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
        if (isPowerShell) {
#endif
          writePowerShellHelpPage();
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
        } else {
          writeZshHelpPage();
        }
#endif
      } else {
        writeHelpPage();
      }
      tmk_freeCommandLineArguments(&commandLineArguments);
      terminate(1);
    } else if (!hasShell && !strcmp(commandLineArguments.utf8Arguments[offset],
                                    "--version")) {
      writeVersionPage();
      tmk_freeCommandLineArguments(&commandLineArguments);
      terminate(1);
    } else if (strlen(commandLineArguments.utf8Arguments[offset]) > 2 &&
               commandLineArguments.utf8Arguments[offset][0] == '-' &&
               commandLineArguments.utf8Arguments[offset][1] == '-') {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
      writeError("the option \"%s\" does not exists%s. Use --help for help "
                 "instructions.",
                 commandLineArguments.utf8Arguments[offset],
                 hasShell ? " for the \"powershell\" shell" : "");
#else
      writeError("the option \"%s\" does not exists%s. Use --help for help "
                 "instructions.",
                 commandLineArguments.utf8Arguments[offset],
                 hasShell ? isPowerShell ? " for the \"powershell\" shell"
                                         : " for the \"zsh\" shell"
                          : "");
#endif
      tmk_freeCommandLineArguments(&commandLineArguments);
      terminate(0);
    }
  }
  if (!hasShell) {
    writeError(
        "invalid shell \"%s\" provided. Use --help for help instructions.",
        commandLineArguments.utf8Arguments[1]);
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  if (commandLineArguments.totalArguments == 2) {
    writeError("no prompt side provided. Use --help for help instructions.");
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  if (!strcmp(commandLineArguments.utf8Arguments[2], "left")) {
    executionArguments->isLeftPrompt = 1;
    executionArguments->referenceLength = INITIAL_LEFT_PROMPT_REFERENCE_LENGTH;
  } else if (!strcmp(commandLineArguments.utf8Arguments[2], "right")) {
    executionArguments->isLeftPrompt = 0;
    executionArguments->referenceLength = INITIAL_RIGHT_PROMPT_REFERENCE_LENGTH;
  } else {
    writeError("invalid prompt side \"%s\" provided. Use --help for help "
               "instructions.",
               commandLineArguments.utf8Arguments[2]);
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  if (!((executionArguments->isPowerShell = isPowerShell))) {
    struct tmk_WindowDimensions windowDimensions;
    tmk_getWindowDimensions(&windowDimensions);
    executionArguments->consoleWidth = windowDimensions.totalColumns;
    tmk_freeCommandLineArguments(&commandLineArguments);
    return;
  }
#endif
  if (commandLineArguments.totalArguments == 3) {
    writeError("no last exit code provided. Use --help for help instructions.");
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  char *conversionStopAddress;
  long exitCode =
      strtol(commandLineArguments.utf8Arguments[3], &conversionStopAddress, 10);
  if (conversionStopAddress == commandLineArguments.utf8Arguments[3] ||
      exitCode < tmk_MINIMUM_EXIT_CODE || exitCode > tmk_MAXIMUM_EXIT_CODE) {
    writeError(
        "invalid exit code \"%d\" provided. Use --help for help instructions.",
        exitCode);
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  executionArguments->exitCode = exitCode;
  if (commandLineArguments.totalArguments == 4) {
    writeError("no console width provided. Use --help for help instructions.");
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  long consoleWidth =
      strtol(commandLineArguments.utf8Arguments[4], &conversionStopAddress, 10);
  if (conversionStopAddress == commandLineArguments.utf8Arguments[4] ||
      consoleWidth <= 0 || consoleWidth > 634) {
    writeError("invalid console width \"%d\" provided. Use --help for help "
               "instructions.",
               consoleWidth);
    tmk_freeCommandLineArguments(&commandLineArguments);
    terminate(0);
  }
  executionArguments->consoleWidth = consoleWidth;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  executionArguments->isAdministrator = isAdministrator();
#else
  executionArguments->userInfo = getpwuid(getuid());
#endif
  tmk_freeCommandLineArguments(&commandLineArguments);
}

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
int isAdministrator(void) {
  SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
  PSID administratorsGroup;
  AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                           DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                           &administratorsGroup);
  BOOL isAdministrator;
  CheckTokenMembership(NULL, administratorsGroup, &isAdministrator);
  FreeSid(administratorsGroup);
  return isAdministrator;
}
#endif

static void writeHelpPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("Usage:");
  tmk_resetFontWeight();
  tmk_write(" %s <", SOFTWARE_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("SHELL");
  tmk_resetFontEffects();
  tmk_write("> [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("ARGUMENTS");
  tmk_resetFontEffects();
  tmk_write("] [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Writes a portion of the shell theme for a specific shell.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE SHELLS");
  tmk_resetFontWeight();
  tmk_writeLine("    powershell     Targets PowerShell.");
  tmk_writeLine("    zsh            Targets ZSH.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("     --help        Shows the software help instructions.");
  tmk_writeLine("     --version     Shows the software version.");
}

static void writePowerShellHelpPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("Usage:");
  tmk_resetFontWeight();
  tmk_write(" %s powershell <", SOFTWARE_NAME);
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
  tmk_writeLine(">");
  tmk_write("       [");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Writes a portion of the shell theme for PowerShell.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE SIDES");
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
static void writeZshHelpPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("Usage:");
  tmk_resetFontWeight();
  tmk_write(" %s zsh <", SOFTWARE_NAME);
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
  tmk_writeLine("Writes a portion of the shell theme for ZSH.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE SIDES");
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

static void writeVersionPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write(SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_writeLine(" %s (running on %s %s)", SOFTWARE_VERSION,
                tmk_OPERATING_SYSTEM, tmk_CPU_ARCHITECTURE);
  tmk_write("%s. Copyright © %d %s <", SOFTWARE_LICENSE, SOFTWARE_CREATION_YEAR,
            SOFTWARE_AUTHOR_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write(SOFTWARE_AUTHOR_EMAIL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
  tmk_writeLine("");
  tmk_write("Software repository available at <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write(SOFTWARE_REPOSITORY_URL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
}

static void writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_setFontAnsiColor(tmk_AnsiColor_DarkRed, tmk_Layer_Foreground);
  tmk_writeError("[ERROR]");
  tmk_resetFontColors();
  tmk_writeError(" ");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeError(SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_writeError(": ");
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
}

static void terminate(int hadSuccess) {
  exit(!hadSuccess);
}

int main(int totalMainArguments, const char **mainArguments) {
  struct ExecutionArguments executionArguments;
  processCommandLineArguments(totalMainArguments, mainArguments,
                              &executionArguments);
  return 0;
}
