/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#define LN(a, b) for (int i = 0; i < s.ws_col; i++) printf(i % 2 ? a : b);
#define ORD(n) !((t->tm_mday - n) % 10)
#ifndef PS
#define PS "/sys/class/power_supply"
#endif

void bat(void);
void cal(struct tm *);
void clk(struct tm *);
void disk(void);
void ip(void);

void
bat(void)
{
	char z[1], w[5] = "0";
	int s = open(PS "/BAT0/status", O_RDONLY),
	    c = open(PS "/BAT0/capacity", O_RDONLY);
	if (s < 0)
		return;
	if (c > 0)
		read(c, w, sizeof(w));
	read(s, z, 1);
	close(s);
	close(c);
	int p = atoi(w);
	printf("%s%s%%f%d%%%%  ", *z == 'C' ? "%F{3}󱐋 " : "", p <= 5 ?
	       "%F{1}  " : p <= 25 ? "%F{3}  " : p <= 50 ? "%F{2}  " :
	       "%F{2}  ", p);
}

void
cal(struct tm *t)
{
	char b[10];
	strftime(b, sizeof(b), "(%a) %b", t);
	printf("%%F{1}󰃭 %%f%s %d%s  ", b, t->tm_mday, ORD(1) ? "st" : ORD(2) ?
	       "nd" : ORD(3) ? "rd" : "th");
}

void
clk(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
	       "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
	       t->tm_min);
}

void
disk(void)
{
	struct statvfs s;
	statvfs("/", &s);
	long unsigned t = s.f_frsize * s.f_blocks, r = s.f_frsize * s.f_bavail;
	int u = ((float)(t - r) / t) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", u < 70 ? 2 : u < 80 ? 3 : 1, u);
}

void
ip(void)
{
	struct ifaddrs *a, *t;
	char z[16] = "127.0.0.1";
	getifaddrs(&a);
	printf("%%F{4} %%f");
	for (t = a; t; t = t->ifa_next)
		if (t->ifa_addr && t->ifa_addr->sa_family & AF_INET &&
		    t->ifa_flags & IFF_RUNNING &&
		    !(t->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in*)t->ifa_addr)->sin_addr,
				  z, sizeof(z));
			break;
		}
	printf("%s  ", z);
	freeifaddrs(a);
}

int
main(void)
{
	time_t e = time(0);
	struct tm t;
	struct winsize s;
	ioctl(2, TIOCGWINSZ, &s);
	localtime_r(&e, &t);
	LN("%%F{1}⊼", "%%F{3}⊵");
	LN("%%F{1}-", "%%F{3}=");
	printf("\n\33[1A%%F{3}:«(");
	ip();
	disk();
	bat();
	cal(&t);
	clk(&t);
	printf("%%F{3})»:\n%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  "
	       "%%F{1}%%~ %%F{6}✗%%f  ");
	return 0;
}
