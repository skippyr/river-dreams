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
#ifndef PS
#define PS "/sys/class/power_supply"
#endif

void bat(void);
void disk(void);
void ip(void);
void tm(void);

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

void
tm(void)
{
	time_t e = time(0);
	struct tm t;
	char b[19];
	localtime_r(&e, &t);
	strftime(b, sizeof(b), "(%a) %b %d %H:%M", &t);
	printf("%s%%f%s", t.tm_hour < 6 ? "%F{6}󰭎 " : t.tm_hour < 12 ?
	       "%F{1}󰖨 " : t.tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", b);
}

int
main(void)
{
	struct winsize s;
	ioctl(2, TIOCGWINSZ, &s);
	LN("%%F{1}⊼", "%%F{3}⊵");
	LN("%%F{1}-", "%%F{3}=");
	printf("\n\33[1A%%F{3}:«(");
	ip();
	disk();
	bat();
	tm();
	printf("%%F{3})»:\n%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  "
	       "%%F{1}%%~%%f ");
	return 0;
}
