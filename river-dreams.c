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

#ifndef BAT_DIR
#define BAT_DIR "/sys/class/power_supply/BAT0"
#endif /* BAT_DIR */
#define IFF_LOOPBACK 0x8
#define IFF_RUNNING 0x40
#define IS_ORD(ord) !((t->tm_mday - ord) % 10)

static void bat(void);
static void blk_usg(void);
static void cal(struct tm *t);
static void clk(struct tm *t);
static void cmd_sep(struct winsize *w);
static int count_dgts(int n);
static void ip(void);
static void mod_sep(struct winsize *w);

static int mod_len_g = 41;

static void
bat(void)
{
	int stat_fd = open(BAT_DIR "/status", O_RDONLY);
	int cap_fd = open(BAT_DIR "/capacity", O_RDONLY);
	char stat_buf[1];
	char cap_buf[5];
	int per;
	if (stat_fd < 0) {
		return;
	} else if (cap_fd > 0) {
		read(cap_fd, cap_buf, sizeof(cap_buf));
	}
	read(stat_fd, stat_buf, sizeof(stat_buf));
	close(stat_fd);
	close(cap_fd);
	per = atoi(cap_buf);
	printf("%s%s%%f%d%%%%  ", *stat_buf == 'C' ? "%F{3}󱐋 " : "", per <= 5 ?
	       "%F{1}  " : per <= 25 ? "%F{3}  " : per <= 50 ? "%F{2}  " :
	       "%F{2}  ", per);
	if (*stat_buf == 'C') {
		mod_len_g += 2;
	}
	mod_len_g += count_dgts(per) + 6;
}

static void
blk_usg(void)
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
	mod_len_g += count_dgts(per);
}

static void
cal(struct tm *t)
{
	char buf[13];
	strftime(buf, sizeof(buf), "(%a) %b %d", t);
	printf("%%F{1}󰃭 %%f%s%s  ", buf, IS_ORD(1) ? "st" : IS_ORD(2) ? "nd" :
	       IS_ORD(3) ? "rd" : "th");
}

static void
clk(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
	       "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
	       t->tm_min);
}

static void
cmd_sep(struct winsize *w)
{
	int i = 0;
	for (; i < w->ws_col; i++) {
		printf(i % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
	}
}

static int
count_dgts(int n)
{
	int i = !n;
	for (; n; n /= 10) {
		i++;
	}
	return i;
}

static void
ip(void)
{
	char buf[16] = "127.0.0.1";
	struct ifaddrs *addr;
	struct ifaddrs *tmp_addr;
	getifaddrs(&addr);
	for (tmp_addr = addr; tmp_addr; tmp_addr = tmp_addr->ifa_next) {
		if (tmp_addr->ifa_addr &&
		    tmp_addr->ifa_addr->sa_family & AF_INET &&
		    tmp_addr->ifa_flags & IFF_RUNNING &&
		    !(tmp_addr->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in*)tmp_addr->ifa_addr)->sin_addr,
				  buf, sizeof(buf));
			break;
		}
	}
	freeifaddrs(addr);
	printf("%%F{4} %%f%s  ", buf);
	mod_len_g += strlen(buf);
}

static void
mod_sep(struct winsize *w)
{
	int i = 0;
	for (; i < w->ws_col - mod_len_g; i++) {
		printf(i % 2 ? "%%F{1}-" : "%%F{3}=");
	}
}

int
main(void)
{
	time_t epoch = time(NULL);
	struct winsize w;
	struct tm t;
	localtime_r(&epoch, &t);
	ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
	cmd_sep(&w);
	printf("%%F{3}:«(");
	ip();
	blk_usg();
	bat();
	cal(&t);
	clk(&t);
	printf("%%F{3})»:");
	mod_sep(&w);
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  "
	       "%%F{1}%%~ %%F{6}✗%%f  ");
	return 0;
}
