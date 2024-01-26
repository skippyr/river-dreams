#define _GNU_SOURCE
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#ifndef BAT
#define BAT "/sys/class/power_supply/BAT0"
#endif
#define ISORD(n) !((t->tm_mday - n) % 10)
#define SYMLN(sym0, sym1, lim)\
	for (i = 0; i < lim; i++)\
		printf(i % 2 ? sym0 : sym1);

static int modlen = 41;

static char *findgitroot(char *buf);
static int countdgts(int n);
static int findrslash(char *path);
static void batmod(void);
static void calmod(struct tm *t);
static void clkmod(struct tm *t);
static void diskmod(void);
static void gitmod(char *root);
static void ipmod(void);
static void pathmod(char *pwd, char *root);
static void dirpermsmod(void);
static void venvmod(void);

static char *
findgitroot(char *buf)
{
	int isroot = 0;
	DIR *d = opendir(buf);
	struct dirent *e;
	int rslash;
	if (d) {
		while ((e = readdir(d)))
			if (!strcmp(e->d_name, ".git")) {
				isroot = 1;
				break;
			}
		closedir(d);
	}
	if (isroot)
		return buf;
	if (strlen(buf) == 1)
		return NULL;
	buf[!(rslash = findrslash(buf)) ? 1 : rslash] = 0;
	return findgitroot(buf);
}

static int
countdgts(int n)
{
	int i;
	for (i = !n; n; n /= 10)
		i++;
	return i;
}

static int
findrslash(char *path)
{
	int i;
	for (i = strlen(path); i; i--)
		if (path[i] == '/')
			return i;
	return 0;
}

static void
batmod(void)
{
	int capfd = open(BAT "/capacity", O_RDONLY);
	int statfd = open(BAT "/status", O_RDONLY);
	char cap[5];
	char stat[1];
	int per;
	if (statfd < 0)
		return;
	if (capfd > 0)
		read(capfd, cap, sizeof(cap));
	read(statfd, stat, sizeof(stat));
	close(statfd);
	close(capfd);
	per = atoi(cap);
	printf("%s%s%%f%d%%%%  ", *stat == 'C' ? "%F{3}󱐋 " : "", per <= 5 ?
	       "%F{1}  " : per <= 25 ? "%F{3}  " : per <= 50 ? "%F{2}  " :
	       "%F{2}  ", per);
	modlen += countdgts(per) + 6 + (*stat == 'C') * 2;
}

static void
calmod(struct tm *t)
{
	char buf[13];
	strftime(buf, sizeof(buf), "(%a) %b %d", t);
	printf("%%F{1}󰃭 %%f%s%s  ", buf, ISORD(1) ? "st" : ISORD(2) ? "nd" :
	       ISORD(3) ? "rd" : "th");
}

static void
clkmod(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
	       "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
	       t->tm_min);
}

static void
diskmod(void)
{
	fsblkcnt_t rem;
	fsblkcnt_t tot;
	int per;
	struct statvfs s;
	statvfs("/", &s);
	tot = s.f_frsize * s.f_blocks;
	rem = s.f_frsize * s.f_bavail;
	per = ((float)(tot - rem) / tot) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", per < 70 ? 2 : per < 80 ? 3 : 1, per);
	modlen += countdgts(per);
}

static void
gitmod(char *root)
{
	int slashes = 0;
	FILE *f;
	char *head;
	char c;
	if (!root)
		return;
	head = malloc(strlen(root) + 11);
	sprintf(head, "%s/.git/HEAD", root);
	f = fopen(head, "r");
	free(head);
	if (!f)
		return;
	printf("%%F{3}:«(%%f");
	while ((c = fgetc(f)) != EOF && c != '\n')
		if (slashes == 2)
			printf("%c", c);
		else if (c == '/')
			slashes++;
	printf("%%F{3})»:");
	fclose(f);
}

static void
ipmod(void)
{
	char ip[16] = "127.0.0.1";
	struct ifaddrs *addr;
	struct ifaddrs *tmpaddr;
	getifaddrs(&addr);
	for (tmpaddr = addr; tmpaddr; tmpaddr = tmpaddr->ifa_next)
		if (tmpaddr->ifa_addr &&
		    tmpaddr->ifa_addr->sa_family & AF_INET &&
		    tmpaddr->ifa_flags & IFF_RUNNING &&
		    !(tmpaddr->ifa_flags & IFF_LOOPBACK)) {
			inet_ntop(AF_INET,
				  &((struct sockaddr_in *)tmpaddr->ifa_addr)->sin_addr,
				  ip, sizeof(ip));
			break;
		}
	freeifaddrs(addr);
	printf("%%F{4} %%f%s  ", ip);
	modlen += strlen(ip);
}

static void
pathmod(char *pwd, char *root)
{
	!root || strlen(root) == 1 ? printf("%%F{1}%%~")
				   : printf("%%F{1}@/%s", pwd + findrslash(root) + 1);
}

static void
dirpermsmod(void)
{
	struct stat s;
	int uid = getuid();
	stat(".", &s);
	printf(!uid || (uid == s.st_uid && s.st_mode & S_IWUSR) ? " %%F{6}✗"
								: " %%F{5}");
}

static void
venvmod(void)
{
	char *v = getenv("VIRTUAL_ENV");
	if (v)
		printf("%%f(%s) ", v + findrslash(v) + 1);
}

int
main(void)
{
	int i;
	struct winsize w;
	time_t tt = time(NULL);
	struct tm *t = localtime(&tt);
	char *pwd = getenv("PWD");
	char *buf = malloc(strlen(pwd) + 1);
	char *root;
	strcpy(buf, pwd);
	root = findgitroot(buf);
	ioctl(2, TIOCGWINSZ, &w);
	SYMLN("%%F{1}⊼", "%%F{3}⊵", w.ws_col);
	printf("%%F{3}:«(");
	ipmod();
	diskmod();
	batmod();
	calmod(t);
	clkmod(t);
	printf("%%F{3})»:");
	SYMLN("%%F{1}-", "%%F{3}=", w.ws_col - modlen);
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  ");
	venvmod();
	pathmod(pwd, root);
	gitmod(root);
	dirpermsmod();
	printf("%%f  \n");
	free(buf);
	return 0;
}
