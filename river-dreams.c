/* See LICENSE for copyright and license details. */
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

#ifndef BATDIR
#define BATDIR "/sys/class/power_supply/BAT0"
#endif
#define ISORD(ord) !((t->tm_mday - ord) % 10)

static unsigned char countdgts(int n);
static void bat(void);
static void blkusg(void);
static void cal(struct tm *t);
static void clk(struct tm *t);
static void cmdsep(struct winsize *w);
static void ip(void);
static void modsep(struct winsize *w);

static unsigned short int modlen = 41;

static unsigned char
countdgts(int n)
{
	unsigned char i;
	for (i = !n; n; n /= 10)
		i++;
	return i;
}

static void
bat(void)
{
	int capfd = open(BATDIR "/capacity", O_RDONLY);
	int statfd = open(BATDIR "/status", O_RDONLY);
	char capbuf[5];
	char statbuf[1];
	unsigned char per;
	if (statfd < 0)
		return;
	else if (capfd > 0)
		read(capfd, capbuf, sizeof(capbuf));
	read(statfd, statbuf, sizeof(statbuf));
	close(statfd);
	close(capfd);
	per = atoi(capbuf);
	printf("%s%s%%f%d%%%%  ", *statbuf == 'C' ? "%F{3}󱐋 " : "", per <= 5 ?
	       "%F{1}  " : per <= 25 ? "%F{3}  " : per <= 50 ? "%F{2}  " :
	       "%F{2}  ", per);
	modlen += countdgts(per) + 6 + (*statbuf == 'C') * 2;
}

static void
blkusg(void)
{
	fsblkcnt_t rem;
	fsblkcnt_t tot;
	unsigned char per;
	struct statvfs fstat;
	statvfs("/", &fstat);
	tot = fstat.f_frsize * fstat.f_blocks;
	rem = fstat.f_frsize * fstat.f_bavail;
	per = ((float)(tot - rem) / tot) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", per < 70 ? 2 : per < 80 ? 3 : 1, per);
	modlen += countdgts(per);
}

static void
cal(struct tm *t)
{
	char buf[13];
	strftime(buf, sizeof(buf), "(%a) %b %d", t);
	printf("%%F{1}󰃭 %%f%s%s  ", buf, ISORD(1) ? "st" : ISORD(2) ? "nd" :
	       ISORD(3) ? "rd" : "th");
}

static void
clk(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
	       "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
	       t->tm_min);
}

static void
cmdsep(struct winsize *w)
{
	unsigned short int i;
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
	for (tmpaddr = addr; tmpaddr; tmpaddr = tmpaddr->ifa_next) {
		if (tmpaddr->ifa_addr &&
		    tmpaddr->ifa_addr->sa_family & AF_INET &&
		    tmpaddr->ifa_flags & IFF_RUNNING &&
		    !(tmpaddr->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in*)tmpaddr->ifa_addr)->sin_addr,
				  buf, sizeof(buf));
			break;
		}
	}
	freeifaddrs(addr);
	printf("%%F{4} %%f%s  ", buf);
	modlen += strlen(buf);
}

static void
modsep(struct winsize *w)
{
	unsigned short int i;
	for (i = 0; i < w->ws_col - modlen; i++)
		printf(i % 2 ? "%%F{1}-" : "%%F{3}=");
}

int
main(void)
{
	time_t epoch = time(NULL);
	struct tm t;
	struct winsize w;
	localtime_r(&epoch, &t);
	ioctl(2, TIOCGWINSZ, &w);
	cmdsep(&w);
	printf("%%F{3}:«(");
	ip();
	blkusg();
	bat();
	cal(&t);
	clk(&t);
	printf("%%F{3})»:");
	modsep(&w);
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  "
	       "%%F{1}%%~ %%F{6}✗%%f  ");
	return 0;
}
