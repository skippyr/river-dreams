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

static void blkusg(void);
static void cmdsep(struct winsize *w);
static int countdgts(int n);
static void ip(void);
static void modsep(struct winsize *w);

static int modlen = 41;

static void
blkusg(void)
{
	struct statvfs fstat;
	unsigned long tot;
	unsigned long rem;
	int per;
	statvfs("/", &fstat);
	tot = fstat.f_frsize * fstat.f_blocks;
	rem = fstat.f_frsize * fstat.f_bavail;
	per = ((float)(tot - rem) / tot) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", per < 70 ? 2 : per < 80 ? 3 : 1, per);
	modlen += countdgts(per);
}

static void
cmdsep(struct winsize *w)
{
	int i;
	for (i = 0; i < w->ws_col; i++)
		printf(i % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

static int
countdgts(int n)
{
	int i;
	for (i = !n; n; n /= 10)
		i++;
	return i;
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

static void
modsep(struct winsize *w)
{
	int i;
	for (i = 0; i < w->ws_col - modlen; i++)
		printf(i % 2 ? "%%F{1}-" : "%%F{3}=");
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
	blkusg();
	printf("%%F{3})»:");
	modsep(&w);
	return 0;
}
