#include "left-prompt.h"

static int countDigits(int number)
{
	int totalDigits;
	for (totalDigits = !number; number; number /= 10) {
		++totalDigits;
	}
	return totalDigits;
}

static char *findGitRoot(const char *pwd, size_t *length)
{
	char *buffer = malloc((*length = strlen(pwd)) + 6);
	if (!buffer) {
		return NULL;
	}
	memcpy(buffer, pwd, *length);
	while (1) {
		memcpy(buffer + *length, "/.git", 6);
		if (!access(buffer, F_OK)) {
			buffer[*length] = 0;
			return buffer;
		}
		if (*length == 1) {
			break;
		}
		buffer[*length] = 0;
		size_t lastSlashOffset = getLastSlashOffset(buffer, *length);
		buffer[(*length = !lastSlashOffset ? 1 : lastSlashOffset)] = 0;
	}
	free(buffer);
	return NULL;
}

static size_t getLastSlashOffset(const char *path, size_t length)
{
	for (size_t offset = length - 1; offset; --offset) {
		if (path[offset] == '/') {
			return offset;
		}
	}
	return 0;
}

static void writeBatteryModule(int *modulesLength)
{
	int statusFile = open(BATTERY "/status", O_RDONLY);
	if (statusFile < 0) {
		return;
	}
	char status;
	read(statusFile, &status, sizeof(status));
	close(statusFile);
	int capacityFile = open(BATTERY "/capacity", O_RDONLY);
	int charge = 0;
	if (capacityFile >= 0) {
		char buffer[4];
		read(capacityFile, buffer, sizeof(buffer));
		close(capacityFile);
		charge = atoi(buffer);
	}
	if (status == 'C' || charge <= 15) {
		printf("%%F{yellow}%s", status == 'C' ? "󱐋 " : "󰀦 ");
		*modulesLength += 2;
	}
	printf("%s%%f%d%%%%  ",
		   charge <= 15   ? "%F{red}󱊡 "
		   : charge <= 50 ? "%F{yellow}󱊢 "
						  : "%F{green}󱊣 ",
		   charge);
	*modulesLength += countDigits(charge) + 5;
}

static void writeCalendarModule(struct tm *localTime)
{
	char buffer[13];
	strftime(buffer, sizeof(buffer), "(%a) %b %d", localTime);
	printf("%%F{red}󰃭 %%f%s%s  ", buffer,
		   IS_DAY_ORDINAL(1)   ? "st"
		   : IS_DAY_ORDINAL(2) ? "nd"
		   : IS_DAY_ORDINAL(3) ? "rd"
							   : "th");
}

static void writeClockModule(struct tm *localTime)
{
	printf("%s%%f%02dh%02dm",
		   localTime->tm_hour < 6    ? "%F{cyan}󰭎 "
		   : localTime->tm_hour < 12 ? "%F{red}󰖨 "
		   : localTime->tm_hour < 18 ? "%F{blue} "
									 : "%F{yellow}󰽥 ",
		   localTime->tm_hour, localTime->tm_min);
}

static void writeCommandLineSeparator(struct winsize *terminalSize)
{
	for (int column = 0; column < terminalSize->ws_col; ++column) {
		printf(column % 2 ? "%%F{red}v" : "%%F{yellow}≥");
	}
	printf("%%F{yellow}:«(");
}

static void writeDirectoryAccessModule(void)
{
	if (access(".", W_OK)) {
		printf("%%F{magenta} ");
	}
}

static void writeDiskModule(int *modulesLength)
{
	struct statvfs64 status;
	statvfs64("/", &status);
	fsblkcnt64_t totalBytes = status.f_frsize * status.f_blocks;
	fsblkcnt64_t availableBytes = status.f_frsize * status.f_bavail;
	int use = (totalBytes - availableBytes) / (float)totalBytes * 100;
	if (use > 85) {
		printf("%%F{yellow}󰀦 ");
		*modulesLength += 2;
	}
	printf("%%F{%s}󰋊 %%f%d%%%%  ",
		   use < 70   ? "green"
		   : use < 80 ? "yellow"
					  : "red",
		   use);
	*modulesLength += countDigits(use);
}

static void writeExitCodeModule(void)
{
	printf("%%F{yellow}{%%(?..%%F{red})%%?%%F{yellow}}");
}

static void writeGitModule(const char *root, size_t length)
{
	if (!root) {
		return;
	}
	char *headPath = malloc(length + 11);
	if (!headPath) {
		return;
	}
	sprintf(headPath, "%s/.git/HEAD", root);
	FILE *head = fopen(headPath, "r");
	free(headPath);
	if (!head) {
		return;
	}
	printf("%%F{yellow}:«(%%f");
	for (int totalSlashes = 0, character = 0;
		 (character = fgetc(head)) != EOF && character != '\n';) {
		if (totalSlashes == 2) {
			putchar(character);
		} else if (character == '/') {
			++totalSlashes;
		}
	}
	printf("%%F{yellow})»");
	fclose(head);
}

static void writeIPModule(int *modulesLength)
{
	char buffer[16] = {0};
	struct ifaddrs *interfacesList;
	getifaddrs(&interfacesList);
	for (struct ifaddrs *interface = interfacesList; !buffer[0] && interface;
		 interface = interface->ifa_next) {
		if (interface->ifa_addr && interface->ifa_addr->sa_family & AF_INET &&
			interface->ifa_flags & IFF_RUNNING &&
			!(interface->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET, &((struct sockaddr_in *)interface->ifa_addr)->sin_addr,
					  buffer, sizeof(buffer));
		}
	}
	freeifaddrs(interfacesList);
	printf("%%F{blue} %%f%s  ", !buffer[0] ? "No Address Found" : buffer);
	*modulesLength += !buffer[0] ? 16 : strlen(buffer);
}

static void writeModulesSeparator(int modulesLength, struct winsize *terminalSize)
{
	printf("%%F{yellow})»:");
	for (int column = 0; column < terminalSize->ws_col - modulesLength; ++column) {
		printf(column % 2 ? "%%F{red}-" : "%%F{yellow}=");
	}
}

static void writePathModule(const char *pwd, const char *gitRoot, size_t gitRootLength)
{
	gitRootLength > 1
		? printf("%%F{red}@%s", pwd + getLastSlashOffset(gitRoot, gitRootLength))
		: printf("%%F{red}%%~");
}

static void writeRootAccessModule(void)
{
	printf("%%F{yellow}%%(#.{%%F{red}#%%F{yellow}}.)");
}

static void writeVenvModule(void)
{
	const char *venv = getenv("VIRTUAL_ENV");
	if (venv) {
		printf("%%f(%s) ", venv + getLastSlashOffset(venv, strlen(venv)) + 1);
	}
}

static void writeZVMModule(char mode)
{
	printf("{%%F{%s}%c%%F{yellow}}",
		   mode == 'n'   ? "yellow"
		   : mode == 'i' ? "green"
		   : mode == 'r' ? "magenta"
						 : "blue",
		   mode - 32);
}

int main(int totalArguments, const char **arguments)
{
	int modulesLength = 41;
	char *pwd = realpath(".", NULL);
	size_t gitRootLength;
	char *gitRoot = findGitRoot(pwd, &gitRootLength);
	time_t epochTime = time(NULL);
	struct tm *localTime = localtime(&epochTime);
	struct winsize terminalSize;
	ioctl(STDERR_FILENO, TIOCGWINSZ, &terminalSize);
	writeCommandLineSeparator(&terminalSize);
	writeIPModule(&modulesLength);
	writeDiskModule(&modulesLength);
	writeBatteryModule(&modulesLength);
	writeCalendarModule(localTime);
	writeClockModule(localTime);
	writeModulesSeparator(modulesLength, &terminalSize);
	writeRootAccessModule();
	writeExitCodeModule();
	if (totalArguments > 1) {
		writeZVMModule(arguments[1][0]);
	}
	printf("⤐  ");
	writeVenvModule();
	writePathModule(pwd, gitRoot, gitRootLength);
	writeGitModule(gitRoot, gitRootLength);
	writeDirectoryAccessModule();
	printf(" %%f\n");
	free(pwd);
	if (gitRoot) {
		free(gitRoot);
	}
	return 0;
}
