#include "left-prompt.hpp"

static int g_modulesLength = 41;

static int countDigits(int number) {
  int totalDigits;
  for (totalDigits = !number; number; number /= 10) {
    ++totalDigits;
  }
  return totalDigits;
}

static std::string findGitRoot(std::string path) {
  while (true) {
    if (!access((path + "/.git").c_str(), F_OK)) {
      return path;
    }
    if (path == "/") {
      return "";
    }
    std::size_t lastSlashOffset = path.find_last_of("/");
    path = !lastSlashOffset ? "/" : path.substr(0, lastSlashOffset);
  }
}

static std::string getDayOrdinal(struct std::tm *localTime) {
  return !((localTime->tm_hour - 1) % 10)   ? "st"
         : !((localTime->tm_hour - 2) % 10) ? "nd"
         : !((localTime->tm_hour - 3) % 10) ? "rd"
                                            : "th";
}

static void writeBatteryModule() {
  int statusFile = open(BATTERY "/status", O_RDONLY);
  if (statusFile < 0) {
    return;
  }
  char status;
  read(statusFile, &status, sizeof(status));
  close(statusFile);
  int capacityFile = open(BATTERY "/capacity", O_RDONLY);
  int charge = 0;
  if (capacityFile > 0) {
    char buffer[4];
    read(capacityFile, buffer, sizeof(buffer));
    close(capacityFile);
    charge = std::atoi(buffer);
  }
  bool isCharging = status == 'C';
  if (isCharging || charge <= 15) {
    std::cout << "%F{yellow}" << (isCharging ? "󱐋 " : "󰀦 ");
    g_modulesLength += 2;
  }
  std::cout << (charge <= 15   ? "%F{red}󱊡 "
                : charge <= 50 ? "%F{yellow}󱊢 "
                               : "%F{green}󱊣 ")
            << "%f" << charge << "%%  ";
  g_modulesLength += countDigits(charge) + 5;
}

static void writeCalendarModule(struct std::tm *localTime) {
  char buffer[13];
  std::strftime(buffer, sizeof(buffer), "(%a) %b %d", localTime);
  std::cout << "%F{red}󰃭 %f" << buffer << getDayOrdinal(localTime) << "  ";
}

static void writeClockModule(struct std::tm *localTime) {
  std::cout << (localTime->tm_hour < 6    ? "%F{cyan}󰭎 "
                : localTime->tm_hour < 12 ? "%F{red}󰖨 "
                : localTime->tm_hour < 18 ? "%F{blue} "
                                          : "%F{yellow}󰽥 ")
            << "%f" << std::setfill('0') << std::setw(2) << localTime->tm_hour
            << "h" << std::setw(2) << localTime->tm_min << "m"
            << std::setfill(' ');
}

static void writeCommandLineSeparator(struct winsize &windowSize) {
  for (int column = 0; column < windowSize.ws_col; ++column) {
    std::cout << (column % 2 ? "%F{red}v" : "%F{yellow}≥");
  }
  std::cout << "%F{yellow}:«(";
}

static void writeCWDAccessModule() {
  if (access(".", W_OK)) {
    std::cout << "%F{blue} ";
  }
}

static void writeCWDPathModule(std::string cwd, std::string &gitRoot) {
  std::cout << "%F{blue}⸖";
  if (gitRoot.length() <= 1) {
    std::string home = getpwuid(getuid())->pw_dir;
    std::string path = !cwd.find(home) ? "~" + cwd.substr(home.length()) : cwd;
    std::cout << "%F{red}";
    writePathSplits(path);
    return;
  }
  std::string repoPath = cwd.substr(gitRoot.find_last_of("/") + 1);
  std::size_t firstSlashOffset = repoPath.find_first_of("/");
  std::string repoName = repoPath.substr(0, firstSlashOffset);
  std::string repoInnerPath;
  try {
    repoInnerPath = repoPath.substr(firstSlashOffset);
  } catch (std::out_of_range &) {
    repoInnerPath = "";
  }
  std::cout << "%F{red}@/%B%F{magenta}" << repoName << "%F{red}%b";
  writePathSplits(repoInnerPath);
}

static void writeDiskModule() {
  struct statvfs64 status;
  statvfs64("/", &status);
  fsblkcnt64_t totalBytes = status.f_frsize * status.f_blocks;
  fsblkcnt64_t availableBytes = status.f_frsize * status.f_bavail;
  int use =
      (totalBytes - availableBytes) / static_cast<float>(totalBytes) * 100;
  if (use > 85) {
    std::cout << "%F{yellow}󰀦 ";
    g_modulesLength += 2;
  }
  std::cout << "%F{"
            << (use < 70   ? "green"
                : use < 80 ? "yellow"
                           : "red")
            << "}󰋊 %f" << use << "%%  ";
  g_modulesLength += countDigits(use);
}

static void writeExitCodeModule() {
  std::cout << "%F{yellow}{%(?..%F{red})%?%F{yellow}}";
}

static void writeGitModule(std::string &gitRoot) {
  if (gitRoot.empty()) {
    return;
  }
  std::FILE *head = std::fopen((gitRoot + "/.git/HEAD").c_str(), "r");
  if (!head) {
    return;
  }
  std::cout << "%F{yellow}:«(%f";
  for (int totalSlashes = 0, character = 0;
       (character = std::fgetc(head)) != EOF && character != '\n';) {
    if (totalSlashes == 2) {
      std::cout << static_cast<char>(character);
    } else if (character == '/') {
      ++totalSlashes;
    }
  }
  std::cout << "%F{yellow})»";
  std::fclose(head);
}

static void writeIPModule() {
  char buffer[16] = {0};
  struct ifaddrs *interfacesList;
  if (getifaddrs(&interfacesList) < 0) {
    goto l_end;
  }
  for (struct ifaddrs *interface = interfacesList; !buffer[0] && interface;
       interface = interface->ifa_next) {
    if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
        interface->ifa_flags & IFF_RUNNING &&
        !(interface->ifa_flags & IFF_LOOPBACK)) {
      inet_ntop(AF_INET,
                &(reinterpret_cast<struct sockaddr_in *>(interface->ifa_addr))
                     ->sin_addr,
                buffer, sizeof(buffer));
      if (!std::strcmp(buffer, "0.0.0.0")) {
        buffer[0] = 0;
      }
    }
  }
  freeifaddrs(interfacesList);
l_end:
  std::cout << "%F{blue} %f" << (!buffer[0] ? "No Address Found" : buffer)
            << "  ";
  g_modulesLength += !buffer[0] ? 16 : std::strlen(buffer);
}

static void writeModulesSeparator(struct winsize &windowSize) {
  std::cout << "%F{yellow})»:";
  for (int column = 0; column < windowSize.ws_col - g_modulesLength; ++column) {
    std::cout << (column % 2 ? "%F{red}-" : "%F{yellow}=");
  }
}

static void writePathSplits(std::string &path) {
  bool hasReadSplit = false;
  for (std::size_t offset = 0, firstSlashOffset = path[0] == '~',
                   lastSlashOffset = path.find_last_of("/");
       offset < path.length(); ++offset) {
    if (offset >= lastSlashOffset) {
      if (offset == lastSlashOffset + 1) {
        std::cout << "%B%F{magenta}";
      }
      std::cout << path[offset];
      continue;
    }
    if (hasReadSplit) {
      if (path[offset] == '/') {
        hasReadSplit = false;
        std::cout << path[offset];
      }
      continue;
    }
    hasReadSplit = offset > firstSlashOffset && path[offset] != '.';
    std::cout << path[offset];
  }
  std::cout << "%b";
}

static void writeRootAccessModule() {
  std::cout << "%F{yellow}%(#.{%F{red}#%F{yellow}}.)";
}

static void writeVirtualEnvModule() {
  const char *virtualEnv = std::getenv("VIRTUAL_ENV");
  if (virtualEnv) {
    std::string virtualEnvStr = virtualEnv;
    std::cout << "%f("
              << virtualEnvStr.substr(virtualEnvStr.find_last_of("/") + 1)
              << ") ";
  }
}

static void writeZVMModule(char mode) {
  std::cout << "{%F{"
            << (mode == 'n'   ? "yellow"
                : mode == 'i' ? "green"
                : mode == 'r' ? "magenta"
                              : "blue")
            << "}" << static_cast<char>(mode - 32) << "%F{yellow}}";
}

int main(int totalArguments, const char **arguments) {
  char *cwd = realpath(".", nullptr);
  std::string gitRoot = findGitRoot(cwd);
  std::time_t epochTime = std::time(nullptr);
  struct std::tm *localTime = std::localtime(&epochTime);
  struct winsize windowSize;
  ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize);
  writeCommandLineSeparator(windowSize);
  writeIPModule();
  writeDiskModule();
  writeBatteryModule();
  writeCalendarModule(localTime);
  writeClockModule(localTime);
  writeModulesSeparator(windowSize);
  writeRootAccessModule();
  writeExitCodeModule();
  if (totalArguments > 1) {
    writeZVMModule(*arguments[1]);
  }
  std::cout << "⤐  ";
  writeVirtualEnvModule();
  writeCWDPathModule(cwd, gitRoot);
  writeGitModule(gitRoot);
  writeCWDAccessModule();
  std::cout << " %f" << std::endl;
  std::free(cwd);
  return 0;
}
