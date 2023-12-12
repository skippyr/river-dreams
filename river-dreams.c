/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#define IFF_LOOPBACK 0x8
#define IFF_RUNNING 0x40

static void cmdsep(struct winsize *w);
static void ip(void);

static int modlen = 41;

static void
cmdsep(struct winsize *w)
{
	int i;
	for (i = 0; i < w->ws_col; i++)
		printf(i % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

static void
ip(void)
{
	char buf[16] = "127.0.0.1";
	struct ifaddrs *addr;
	struct ifaddrs *tmpaddr;
	getifaddrs(&addr);
	for (tmpaddr = addr; tmpaddr; tmpaddr = tmpaddr->ifa_next)
		if (tmpaddr->ifa_addr &&
		    tmpaddr->ifa_addr->sa_family & AF_INET &&
		    tmpaddr->ifa_flags & IFF_RUNNING &&
		    !(tmpaddr->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in*)tmpaddr->ifa_addr)->sin_addr,
				  buf, sizeof(buf));
			break;
		}
	freeifaddrs(addr);
	printf("%%F{4} %%f%s  ", buf);
	modlen += strlen(buf);
}

int
main(void)
{
	time_t epoch = time(NULL);
	struct winsize w;
	struct tm t;
	localtime_r(&epoch, &t);
	ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
	cmdsep(&w);
	printf("%%F{3}:«(");
	ip();
	return 0;
}
