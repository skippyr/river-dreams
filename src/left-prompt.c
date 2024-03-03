#define _GNU_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#ifndef BATTERY
#define BATTERY "/sys/class/power_supply/BAT0"
#endif
#define IS_ORDINAL_DAY(a_ordinal) !((date->tm_mday - a_ordinal) % 10)

static int CountDigits(int number);
static size_t FindLastSlash(const char* path, size_t length);
static void FindGitRoot(const char* pwd, char** root, size_t* length);
static void WriteBatteryCharge(void);
static void WriteCalendar(struct tm* date);
static void WriteClock(struct tm* date);
static void WriteCommandsSeparator(struct winsize* windowSize);
static void WriteDirectoryAccess(void);
static void WriteDiskUsage(void);
static void WriteExitCode(void);
static void WriteGitBranch(const char* root, size_t length);
static void WriteLocalIPV4Address(void);
static void WriteModulesSeparator(struct winsize* windowSize);
static void WritePath(const char* pwd, const char* gitRoot, size_t gitRootLength);
static void WriteRootUserStatus(void);
static void WriteVirtualEnvironment(void);

static int g_modulesLength = 41;

static int CountDigits(int number)
{
	int digits;
	for (digits = !number; number; number /= 10)
	{
		++digits;
	}
	return (digits);
}

static size_t FindLastSlash(const char* path, size_t length)
{
	size_t index;
	for (index = length - 1; index; --index)
	{
		if (path[index] == '/')
		{
			return (index);
		}
	}
	return (0);
}

static void FindGitRoot(const char* pwd, char** root, size_t* length)
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
		*(*root + (!(*length = FindLastSlash(*root, *length)) ? ++*length : *length)) = 0;
	}
	free(*root);
	*root = NULL;
}

static void WriteBatteryCharge(void)
{
	int capacityFile = open(BATTERY "/capacity", O_RDONLY);
	int statusFile = open(BATTERY "/status", O_RDONLY);
	char capacityBuffer[5];
	char statusBuffer;
	int charge = 0;
	if (statusFile < 0)
	{
		return;
	}
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
	g_modulesLength += CountDigits(charge) + 6 + (statusBuffer == 'C') * 2;
}

static void WriteGitBranch(const char* root, size_t length)
{
	char* path;
	FILE* head;
	int character;
	int slashes = 0;
	if (!root)
	{
		return;
	}
	path = malloc(length + 11);
	sprintf(path, "%s/.git/HEAD", root);
	head = fopen(path, "r");
	free(path);
	if (!head)
	{
		return;
	}
	printf("%%F{3}:«(%%f");
	while ((character = fgetc(head)) != EOF && character != '\n')
	{
		if (slashes == 2)
		{
			putchar(character);
		}
		else if (character == '/')
		{
			++slashes;
		}
	}
	printf("%%F{3})»");
	fclose(head);
}

static void WriteCalendar(struct tm* date)
{
	char buffer[13];
	strftime(buffer, sizeof(buffer), "(%a) %b %d", date);
	printf("%%F{red}󰃭 %%f%s%s  ", buffer,
	       IS_ORDINAL_DAY(1)   ? "st"
	       : IS_ORDINAL_DAY(2) ? "nd"
	       : IS_ORDINAL_DAY(3) ? "rd"
	                           : "th");
}

static void WriteClock(struct tm* date)
{
	printf("%s%%f%02dh%02dm",
	       date->tm_hour < 6    ? "%F{cyan}󰭎 "
	       : date->tm_hour < 12 ? "%F{red}󰖨 "
	       : date->tm_hour < 18 ? "%F{blue} "
	                            : "%F{yellow}󰽥 ",
	       date->tm_hour, date->tm_min);
}

static void WriteCommandsSeparator(struct winsize* windowSize)
{
	int column;
	for (column = 0; column < windowSize->ws_col; ++column)
	{
		printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
	}
	printf("%%F{yellow}:«(");
}

static void WriteDirectoryAccess(void)
{
	printf(access(".", W_OK) ? "%%F{magenta} " : "");
}

static void WriteDiskUsage(void)
{
	fsblkcnt64_t totalBytes;
	fsblkcnt64_t availableBytes;
	int usage;
	struct statvfs64 status;
	statvfs64("/", &status);
	totalBytes = status.f_frsize * status.f_blocks;
	availableBytes = status.f_frsize * status.f_bavail;
	usage = ((totalBytes - availableBytes) / (float)totalBytes) * 100;
	printf("%%F{%s}󰋊 %%f%d%%%%  ",
	       usage < 70   ? "green"
	       : usage < 80 ? "yellow"
	                    : "red",
	       usage);
	g_modulesLength += CountDigits(usage);
}

static void WriteExitCode(void)
{
	printf("{%%(?..%%F{1})%%?%%F{3}}⤐  ");
}

static void WriteLocalIPV4Address(void)
{
	char buffer[16] = "127.0.0.1";
	struct ifaddrs* interfacesList;
	struct ifaddrs* interface;
	getifaddrs(&interfacesList);
	for (interface = interfacesList; interface; interface = interface->ifa_next)
	{
		if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
		    interface->ifa_flags & IFF_RUNNING && !(interface->ifa_flags & IFF_LOOPBACK))
		{
			inet_ntop(AF_INET, &((struct sockaddr_in*)interface->ifa_addr)->sin_addr,
			          buffer, sizeof(buffer));
			break;
		}
	}
	freeifaddrs(interfacesList);
	printf("%%F{blue} %%f%s  ", buffer);
	g_modulesLength += strlen(buffer);
}

static void WriteModulesSeparator(struct winsize* windowSize)
{
	int column;
	printf("%%F{yellow})»:");
	for (column = 0; column < windowSize->ws_col - g_modulesLength; ++column)
	{
		printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
	}
}

static void WritePath(const char* pwd, const char* gitRoot, size_t gitRootLength)
{
	!gitRoot || !gitRoot[1]
		? printf("%%F{1}%%~")
		: printf("%%F{1}@/%s", pwd + FindLastSlash(gitRoot, gitRootLength) + 1);
}

static void WriteRootUserStatus(void)
{
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void WriteVirtualEnvironment(void)
{
	const char* path = getenv("VIRTUAL_ENV");
	if (path)
	{
		printf("%%f(%s) ", path + FindLastSlash(path, strlen(path)) + 1);
	}
}

int main(void)
{
	const char* pwd = getenv("PWD");
	char* gitRoot;
	size_t gitRootLength;
	time_t epoch = time(NULL);
	struct tm* date = localtime(&epoch);
	struct winsize windowSize;
	ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize);
	FindGitRoot(pwd, &gitRoot, &gitRootLength);
	WriteCommandsSeparator(&windowSize);
	WriteLocalIPV4Address();
	WriteDiskUsage();
	WriteBatteryCharge();
	WriteCalendar(date);
	WriteClock(date);
	WriteModulesSeparator(&windowSize);
	WriteRootUserStatus();
	WriteExitCode();
	WriteVirtualEnvironment();
	WritePath(pwd, gitRoot, gitRootLength);
	WriteGitBranch(gitRoot, gitRootLength);
	WriteDirectoryAccess();
	printf(" %%f\n");
	if (gitRoot)
	{
		free(gitRoot);
	}
	return (0);
}
