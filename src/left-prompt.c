#include "left-prompt.h"

static int g_modulesLength = 41;

static int countDigits(int number)
{
    int totalDigits;
    for (totalDigits = !number; number; number /= 10)
    {
        ++totalDigits;
    }
    return totalDigits;
}

static void findGitRoot(const char* pwd, char** root, size_t* length)
{
    size_t pwdLength = strlen(pwd);
    *root = malloc(pwdLength + 6);
    for (*length = 0; *length < pwdLength; ++*length)
    {
        *(*root + *length) = pwd[*length];
    }
    while (1)
    {
        *(*root + *length) = '/';
        *(*root + ++*length) = '.';
        *(*root + ++*length) = 'g';
        *(*root + ++*length) = 'i';
        *(*root + ++*length) = 't';
        *(*root + ++*length) = 0;
        if (!access(*root, F_OK))
        {
            *(*root + (*length -= 5)) = 0;
            return;
        }
        *(*root + (*length -= 5)) = 0;
        if (!*(*root + 1))
        {
            break;
        }
        *(*root + (!(*length = findLastSlash(*root, *length)) ? ++*length : *length)) = 0;
    }
    free(*root);
    *root = NULL;
}

static size_t findLastSlash(const char* path, size_t length)
{
    for (size_t index = length - 1; index; --index)
    {
        if (path[index] == '/')
        {
            return index;
        }
    }
    return 0;
}

static void writeBatteryModule(void)
{
    int statusFile = open(BATTERY "/status", O_RDONLY);
    if (statusFile < 0)
    {
        return;
    }
    int capacityFile = open(BATTERY "/capacity", O_RDONLY);
    char statusBuffer;
    char capacityBuffer[5];
    int charge = 0;
    if (capacityFile > 0)
    {
        read(capacityFile, capacityBuffer, sizeof(capacityBuffer));
        close(capacityFile);
        charge = atoi(capacityBuffer);
    }
    read(statusFile, &statusBuffer, sizeof(statusBuffer));
    close(statusFile);
    printf("%s%s%%f%d%%%%  ", statusBuffer == 'C' ? "%F{yellow}󱐋 " : "",
           charge <= 5    ? "%F{red}  "
           : charge <= 25 ? "%F{yellow}  "
           : charge <= 50 ? "%F{green}  "
                          : "%F{green}  ",
           charge);
    g_modulesLength += countDigits(charge) + 6 + (statusBuffer == 'C') * 2;
}

static void writeCalendarModule(struct tm* date)
{
    char buffer[13];
    strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
    printf("%%F{red}󰃭 %%f%s%s  ", buffer,
           !((date->tm_mday - 1) % 10)   ? "st"
           : !((date->tm_mday - 2) % 10) ? "nd"
           : !((date->tm_mday - 3) % 10) ? "rd"
                                         : "th");
}

static void writeClockModule(struct tm* date)
{
    printf("%s%%f%02dh%02dm",
           date->tm_hour < 6    ? "%F{cyan}󰭎 "
           : date->tm_hour < 12 ? "%F{red}󰖨 "
           : date->tm_hour < 18 ? "%F{blue} "
                                : "%F{yellow}󰽥 ",
           date->tm_hour, date->tm_min);
}

static void writeCommandsSeparator(struct winsize* windowSize)
{
    for (int column = 0; column < windowSize->ws_col; ++column)
    {
        printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
    }
    printf("%%F{yellow}:«(");
}

static void writeDirectoryAccessModule(void)
{
    printf(access(".", W_OK) ? "%%F{magenta} " : "");
}

static void writeDiskModule(void)
{
    struct statvfs64 status;
    statvfs64("/", &status);
    fsblkcnt64_t totalBytes = status.f_frsize * status.f_blocks;
    fsblkcnt64_t availableBytes = status.f_frsize * status.f_bavail;
    int usage = ((totalBytes - availableBytes) / (float)totalBytes) * 100;
    printf("%%F{%s}󰋊 %%f%d%%%%  ", usage < 70 ? "green" : usage < 80 ? "yellow" : "red", usage);
    g_modulesLength += countDigits(usage);
}

static void writeExitCodeModule(void)
{
    printf("{%%(?..%%F{red})%%?%%F{yellow}}⤐  ");
}

static void writeGitBranchModule(const char* root, size_t length)
{
    if (!root)
    {
        return;
    }
    char* path = malloc(length + 11);
    sprintf(path, "%s/.git/HEAD", root);
    FILE* head = fopen(path, "r");
    free(path);
    if (!head)
    {
        return;
    }
    printf("%%F{yellow}:«(%%f");
    for (int character, totalSlashes = 0; (character = fgetc(head)) != EOF && character != '\n';)
    {
        if (totalSlashes == 2)
        {
            putchar(character);
        }
        else if (character == '/')
        {
            ++totalSlashes;
        }
    }
    printf("%%F{yellow})»");
    fclose(head);
}

static void writeIPModule(void)
{
    char ip[16] = "127.0.0.1";
    struct ifaddrs* interfacesList;
    getifaddrs(&interfacesList);
    for (struct ifaddrs* interface = interfacesList; interface; interface = interface->ifa_next)
    {
        if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET && interface->ifa_flags & IFF_RUNNING &&
            !(interface->ifa_flags & IFF_LOOPBACK))
        {
            inet_ntop(AF_INET, &((struct sockaddr_in*)interface->ifa_addr)->sin_addr, ip, sizeof(ip));
            break;
        }
    }
    freeifaddrs(interfacesList);
    printf("%%F{blue} %%f%s  ", ip);
    g_modulesLength += strlen(ip);
}

static void writeModulesSeparator(struct winsize* windowSize)
{
    printf("%%F{yellow})»:");
    for (int column = 0; column < windowSize->ws_col - g_modulesLength; ++column)
    {
        printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
    }
}

static void writePathModule(const char* pwd, const char* gitRoot, size_t gitRootLength)
{
    !gitRoot || !gitRoot[1] ? printf("%%F{red}%%~")
                            : printf("%%F{red}@/%s", pwd + findLastSlash(gitRoot, gitRootLength) + 1);
}

static void writeRootUserModule(void)
{
    printf("%%F{yellow}%%(#.{%%F{red}#%%F{yellow}}.)");
}

static void writeVirtualEnvModule(void)
{
    const char* path = getenv("VIRTUAL_ENV");
    if (path)
    {
        printf("%%f(%s) ", path + findLastSlash(path, strlen(path)) + 1);
    }
}

int main(void)
{
    time_t epoch = time(NULL);
    struct tm* date = localtime(&epoch);
    const char* pwd = getenv("PWD");
    char* gitRoot;
    size_t gitRootLength;
    struct winsize windowSize;
    ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize);
    findGitRoot(pwd, &gitRoot, &gitRootLength);
    writeCommandsSeparator(&windowSize);
    writeIPModule();
    writeDiskModule();
    writeBatteryModule();
    writeCalendarModule(date);
    writeClockModule(date);
    writeModulesSeparator(&windowSize);
    writeRootUserModule();
    writeExitCodeModule();
    writeVirtualEnvModule();
    writePathModule(pwd, gitRoot, gitRootLength);
    writeGitBranchModule(gitRoot, gitRootLength);
    writeDirectoryAccessModule();
    printf(" %%f\n");
    if (gitRoot)
    {
        free(gitRoot);
    }
    return 0;
}
