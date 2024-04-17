#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <unistd.h>

#ifndef BAT
#define BAT "/sys/class/power_supply/BAT0"
#endif

static unsigned short countdigits(int n);
static char *findgitroot(char *pwd, size_t *len);
static size_t getrslashpos(char *str, size_t len);
static void writebatper(void);
static void writecalendar(struct tm *t);
static void writeclock(struct tm *t);
static void writecmdsep(struct winsize *w);
static void writediracc(void);
static void writediskuse(void);
static void writeexitcd(void);
static void writegitbranch(char *root, size_t len);
static void writeip(void);
static void writemodsep(struct winsize *w);
static void writepath(char *pwd, char *gitroot, size_t gitrootlen);
static void writerootstat(void);
static void writevenv(void);

static unsigned short modlen_g = 41;

static unsigned short
countdigits(int n)
{
	unsigned short i;
	for (i = !n; n; n /= 10)
		++i;
	return i;
}

static char *
findgitroot(char *pwd, size_t *len)
{
	char *buf = malloc((*len = strlen(pwd)) + 6);
	size_t rslash;
	memcpy(buf, pwd, *len);
	while (1) {
		buf[*len] = '/';
		*(int *)(buf + *len + 1) = *(int *)".git";
		buf[*len + 5] = 0;
		if (!access(buf, F_OK)) {
			buf[*len] = 0;
			return buf;
		}
		if (*len == 1)
			break;
		buf[*len] = 0;
		rslash = getrslashpos(buf, *len);
		buf[(*len = !rslash ? 1 : rslash)] = 0;
	}
	free(buf);
	return NULL;
}

static size_t
getrslashpos(char *str, size_t len)
{
	size_t i;
	for (i = len - 1; i; --i)
		if (str[i] == '/')
			return i;
	return 0;
}

static void
writebatper(void)
{
	int statfd = open(BAT "/status", O_RDONLY);
	int capfd;
	int per = 0;
	char statbuf;
	char capbuf[5];
	if (statfd < 0)
		return;
	if ((capfd = open(BAT "/capacity", O_RDONLY)) > 0) {
		read(capfd, capbuf, sizeof(capbuf));
		close(capfd);
		per = atoi(capbuf);
	}
	read(statfd, &statbuf, sizeof(statbuf));
	close(statfd);
	if (statbuf == 'C') {
		printf("%%F{3}󱐋 ");
		modlen_g += 2;
	}
	printf("%s%%f%d%%%%  ", per <= 15 ? "%F{1}  " : per <= 35 ? "%F{3}  " :
		   per <= 70 ? "%F{2}  " : "%F{2}  ", per);
	modlen_g += countdigits(per) + 6;
}

static void
writecalendar(struct tm *t)
{
	char buf[13];
	strftime(buf, sizeof(buf), "(%a) %b %d", t);
	printf("%%F{1}󰃭 %%f%s%s  ", buf, !((t->tm_mday - 1) % 10) ? "st" :
		   !((t->tm_mday - 2) % 10) ? "nd" : !((t->tm_mday - 3) % 10) ? "rd" :
		   "th");
}

static void
writeclock(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
		   "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
		   t->tm_min);
}

static void
writecmdsep(struct winsize *w)
{
	unsigned short i;
	for (i = 0; i < w->ws_col; ++i)
		printf(i % 2 ? "%%F{1}v" : "%%F{3}≥");
	printf("%%F{3}:«(");
}

static void
writediracc(void)
{
	if (access(".", W_OK))
		printf("%%F{5} ");
}

static void
writediskuse(void)
{
	struct statvfs64 s;
	fsblkcnt64_t tot;
	fsblkcnt64_t avl;
	int use;
	statvfs64("/", &s);
	tot = s.f_frsize * s.f_blocks;
	avl = s.f_frsize * s.f_bavail;
	use = ((tot - avl) / (float)tot) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", use < 70 ? 2 : use < 80 ? 3 : 1, use);
	modlen_g += countdigits(use);
}

static void
writeexitcd(void)
{
	printf("{%%(?..%%F{1})%%?%%F{3}}⤐  ");
}

static void
writegitbranch(char *root, size_t len)
{
	char *headpath;
	FILE *head;
	int c;
	size_t i;
	if (!root)
		return;
	headpath = malloc(len + 11);
	sprintf(headpath, "%s/.git/HEAD", root);
	head = fopen(headpath, "r");
	free(headpath);
	if (!head)
		return;
	printf("%%F{3}:«(%%f");
	for (i = 0; (c = fgetc(head)) != EOF && c != '\n';)
		if (i == 2)
			putchar(c);
		else if (c == '/')
			++i;
	printf("%%F{3})»");
	fclose(head);
}

static void
writeip(void)
{
	char buf[16] = "127.0.0.1";
	struct ifaddrs *list;
	struct ifaddrs *addr;
	getifaddrs(&list);
	for (addr = list; addr; addr = addr->ifa_next)
		if (addr->ifa_addr && addr->ifa_addr->sa_family & AF_INET &&
			addr->ifa_flags & IFF_RUNNING &&
			!(addr->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET, &((struct sockaddr_in*)addr->ifa_addr)->sin_addr,
					  buf, sizeof(buf));
			break;
		}
	freeifaddrs(list);
	printf("%%F{4} %%f%s  ", buf);
	modlen_g += strlen(buf);
}

static void
writemodsep(struct winsize *w)
{
	unsigned short i;
	printf("%%F{3})»:");
	for (i = 0; i < w->ws_col - modlen_g; ++i)
		printf(i % 2 ? "%%F{1}-" : "%%F{3}=");
}

static void
writepath(char *pwd, char *gitroot, size_t gitrootlen)
{
	gitrootlen > 1 ?
	printf("%%F{1}@%s", pwd + getrslashpos(gitroot, gitrootlen)) :
	printf("%%F{1}%%~");
}

static void
writerootstat(void)
{
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void
writevenv(void)
{
	char *venv = getenv("VIRTUAL_ENV");
	if (venv)
		printf("%%f(%s) ", venv + getrslashpos(venv, strlen(venv)) + 1);
}

int
main(void)
{
	time_t tt = time(NULL);
	struct tm *t = localtime(&tt);
	struct winsize w;
	char *pwd = getenv("PWD");
	size_t gitrootlen;
	char *gitroot = findgitroot(pwd, &gitrootlen);
	ioctl(2, TIOCGWINSZ, &w);
	writecmdsep(&w);
	writeip();
	writediskuse();
	writebatper();
	writecalendar(t);
	writeclock(t);
	writemodsep(&w);
	writerootstat();
	writeexitcd();
	writevenv();
	writepath(pwd, gitroot, gitrootlen);
	writegitbranch(gitroot, gitrootlen);
	writediracc();
	if (gitroot)
		free(gitroot);
	printf(" %%f\n");
	return 0;
}
