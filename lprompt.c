#include <ifaddrs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>

#define ISORD(n) !((t.tm_mday - n) % 10)
#define ISDWN t.tm_hour < 6
#define ISMRN t.tm_hour < 12
#define ISAFT t.tm_hour < 18

static void
writevsep(void)
{
	struct winsize s;
	ioctl(2, TIOCGWINSZ, &s);
	for (; s.ws_col; s.ws_col--)
		printf(s.ws_col % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

static void
writehsep(void)
{
	printf("  ");
}

static void
writeip(void)
{
	struct ifaddrs *a;
	char z[16] = "127.0.0.1";
	printf("%%F{4} %%f");
	getifaddrs(&a);
	for (struct ifaddrs *t = a; t; t = t->ifa_next)
		if (t->ifa_addr && t->ifa_addr->sa_family & AF_INET &&
		    t->ifa_flags & IFF_RUNNING &&
		    !(t->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in*)t->ifa_addr)->sin_addr,
				  z, sizeof(z));
			break;
		}
	printf("%s", z);
	freeifaddrs(a);
}

static void
writedisk(void)
{
	struct statvfs s;
	statvfs("/", &s);
	unsigned long t = s.f_frsize * s.f_blocks, r = s.f_frsize * s.f_bavail;
	int u = ((float)(t - r) / t) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%", u < 70 ? 2 : u < 80 ? 3 : 1, u);
}

static void
writetm(void)
{
	time_t e = time(0);
	struct tm t;
	char *w[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},
	     *m[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
		     "Sep", "Oct", "Nov", "Dec"};
	localtime_r(&e, &t);
	printf("%%F{%d}%s%%f(%s) %s %d%s %02dh%02dm", ISDWN ? 5 : ISMRN ? 1 :
	       ISAFT ? 4 : 3, ISDWN ? "󰭎 " : ISMRN ? "󰖨 " : ISAFT ? " " : "󰽥 ",
	       w[t.tm_wday], m[t.tm_mon], t.tm_mday, ISORD(1) ? "st" :
	       ISORD(2) ? "nd" : ISORD(3) ? "rd" : "st", t.tm_hour, t.tm_min);
}

int
main(void)
{
	writevsep();
	printf("%%F{3}:«(");
	writeip();
	writehsep();
	writedisk();
	writehsep();
	writetm();
	printf("%%F{3})»:\n⤐  %%F{1}%%1~%%f ");
	return 0;
}
