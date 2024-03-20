#include "left-prompt.hpp"

static int g_modulesLength = 41;

static int countDigits(int number) {
  int digits;
  for (digits = !number; number; number /= 10) {
    ++digits;
  }
  return digits;
}

static std::string findGitRoot(std::string path) {
  if (!access((path + "/.git").c_str(), F_OK)) {
    return path;
  }
  if (path == "/") {
    return "";
  }
  size_t lastSlash = path.find_last_of('/');
  return findGitRoot(!lastSlash ? "/" : path.substr(0, lastSlash));
}

static void writeBatteryModule() {
  int statusFile = open("/sys/class/power_supply/BAT0/status", O_RDONLY);
  if (statusFile < 0) {
    return;
  }
  int capacityFile = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY);
  char statusBuffer;
  char capacityBuffer[5];
  int charge = 0;
  if (capacityFile > 0) {
    read(capacityFile, capacityBuffer, sizeof(capacityBuffer));
    close(capacityFile);
    charge = std::atoi(capacityBuffer);
  }
  read(statusFile, &statusBuffer, sizeof(statusBuffer));
  close(statusFile);
  bool isCharging = statusBuffer == 'C';
  std::cout << (isCharging ? "%F{yellow}󱐋 " : "")
            << (charge <= 5    ? "%F{red}  "
                : charge <= 25 ? "%F{yellow}  "
                : charge <= 50 ? "%F{green}  "
                               : "%F{green}  ")
            << "%f" << charge << "%%  ";
  g_modulesLength += countDigits(charge) + 6 + isCharging * 2;
}

static void writeCalendar(struct tm *date) {
  char buffer[13];
  std::strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
  std::cout << "%F{red}󰃭 %f" << buffer
            << (!((date->tm_mday - 1) % 10)   ? "st"
                : !((date->tm_mday - 2) % 10) ? "nd"
                : !((date->tm_mday - 3) % 10) ? "rd"
                                              : "th")
            << "  ";
}

static void writeClock(struct tm *date) {
  std::cout << (date->tm_hour < 6    ? "%F{cyan}󰭎 "
                : date->tm_hour < 12 ? "%F{red}󰖨 "
                : date->tm_hour < 18 ? "%F{blue} "
                                     : "%F{yellow}󰽥 ")
            << "%f" << std::setw(2) << std::setfill('0') << date->tm_hour << 'h'
            << std::setw(2) << std::setfill('0') << date->tm_min << 'm';
}

static void writeCommandsSeparator(struct winsize &windowSize) {
  for (int column = 0; column < windowSize.ws_col; ++column) {
    std::cout << (column % 2 ? "%F{red}v" : "%F{yellow}≥");
  }
  std::cout << "%F{yellow}:«(";
}

static void writeDirectoryAccessModule() {
  std::cout << (access(".", W_OK) ? "%F{magenta} " : "");
}

static void writeDiskModule() {
  struct statvfs64 status;
  statvfs64("/", &status);
  fsblkcnt64_t totalBytes = status.f_frsize * status.f_blocks;
  fsblkcnt64_t availableBytes = status.f_frsize * status.f_bavail;
  int usage =
      ((totalBytes - availableBytes) / static_cast<float>(totalBytes)) * 100;
  std::cout << "%F{"
            << (usage < 70   ? "green"
                : usage < 80 ? "yellow"
                             : "red")
            << "}󰋊 %f" << usage << "%%  ";
  g_modulesLength += countDigits(usage);
}

static void writeExitCodeModule() {
  std::cout << "{%(?..%F{red})%?%F{yellow}}⤐  ";
}

static void writeGitBranchModule(std::string &gitRoot) {
  if (gitRoot == "") {
    return;
  }
  std::FILE *head = std::fopen((gitRoot + "/.git/HEAD").c_str(), "r");
  std::cout << "%F{yellow}:«(%f";
  int slashes = 0;
  for (char character;
       (character = std::fgetc(head)) != EOF && character != '\n';) {
    if (slashes == 2) {
      std::cout << character;
    } else if (character == '/') {
      ++slashes;
    }
  }
  std::cout << "%F{yellow})»";
  std::fclose(head);
}

static void writeIPModule() {
  char ip[16] = "127.0.0.1";
  struct ifaddrs *interfacesList;
  getifaddrs(&interfacesList);
  for (struct ifaddrs *interface = interfacesList; interface;
       interface = interface->ifa_next) {
    if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
        interface->ifa_flags & IFF_RUNNING &&
        !(interface->ifa_flags & IFF_LOOPBACK)) {
      inet_ntop(AF_INET,
                &reinterpret_cast<struct sockaddr_in *>(interface->ifa_addr)
                     ->sin_addr,
                ip, sizeof(ip));
      break;
    }
  }
  freeifaddrs(interfacesList);
  std::cout << "%F{blue} %f" << ip << "  ";
  g_modulesLength += std::strlen(ip);
}

static void writeModulesSeparator(struct winsize &windowSize) {
  std::cout << "%F{yellow})»:";
  for (int column = 0; column < windowSize.ws_col - g_modulesLength; ++column) {
    std::cout << (column % 2 ? "%F{red}-" : "%F{yellow}=");
  }
}

static void writePath(std::string &pwd, std::string &gitRoot) {
  std::cout << "%F{red}"
            << (gitRoot == "" || gitRoot == "/"
                    ? "%~"
                    : "@/" + pwd.substr(gitRoot.find_last_of('/') + 1));
}

static void writeRootUserModule() {
  std::cout << "%F{yellow}%(#.{%F{red}#%F{yellow}}.)";
}

static void writeVirtualEnvModule() {
  const char *virtualEnvChar = std::getenv("VIRTUAL_ENV");
  if (virtualEnvChar) {
    std::string virtualEnvStr = virtualEnvChar;
    std::cout << virtualEnvStr.substr(virtualEnvStr.find_last_of('/') + 1);
  }
}

int main() {
  std::string pwd = std::getenv("PWD");
  std::time_t epoch = std::time(nullptr);
  std::string gitRoot = findGitRoot(pwd);
  struct std::tm *date = std::localtime(&epoch);
  struct winsize windowSize;
  ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize);
  writeCommandsSeparator(windowSize);
  writeIPModule();
  writeDiskModule();
  writeBatteryModule();
  writeCalendar(date);
  writeClock(date);
  writeModulesSeparator(windowSize);
  writeRootUserModule();
  writeExitCodeModule();
  writeVirtualEnvModule();
  writePath(pwd, gitRoot);
  writeGitBranchModule(gitRoot);
  writeDirectoryAccessModule();
  std::cout << " %f" << std::endl;
  return 0;
}
