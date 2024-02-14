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

#ifndef BAT
#define BAT "/sys/class/power_supply/BAT0"
#endif
#define ISORD(n) !((t->tm_mday - n) % 10)
#define SYMLN(len, sym0, sym1) \
	for (i = 0; i < len; i++) \
		printf(i % 2 ? sym0 : sym1);

static void bat(void);
static void branch(char *gitroot, size_t gitrootlen);
static void cal(struct tm *t);
static void clk(struct tm *t);
static void cmdsep(struct winsize *w);
static int cntdgts(int n);
static void dirwperms(void);
static void disk(void);
static void exitstat(void);
static void findgitroot(char *pwd, char **root, size_t *rootlen);
static size_t findrslash(size_t len, char *path);
static void ip(void);
static void modsep(struct winsize *w);
static void path(char *pwd, char *gitroot, size_t gitrootlen);
static void rootstat(void);
static void venv(void);

static int modlen_g = 41;

static void bat(void)
{
	int statfd = open(BAT "/status", O_RDONLY);
	int capfd;
	char statbuf[1];
	char capbuf[5];
	int rem;
	if (statfd < 0)
		return;
	capfd = open(BAT "/capacity", O_RDONLY);
	read(statfd, statbuf, sizeof(statbuf));
	read(capfd, capbuf, sizeof(capbuf));
	close(statfd);
	close(capfd);
	rem = atoi(capbuf);
	printf("%s%s%%f%d%%%%  ", *statbuf == 'C' ? "%F{3}󱐋 " : "", rem <= 5 ?
		   "%F{1}  " : rem <= 25 ? "%F{3}  " : rem <= 50 ? "%F{2}  " :
		   "%F{2}  ", rem);
	modlen_g += cntdgts(rem) + 6 + (*statbuf == 'C') * 2;
}

static void branch(char *gitroot, size_t gitrootlen)
{
	char *headpath;
	FILE *head;
	int slashes = 0;
	int c;
	if (!gitroot)
		return;
	headpath = malloc(gitrootlen + 11);
	sprintf(headpath, "%s/.git/HEAD", gitroot);
	head = fopen(headpath, "r");
	free(headpath);
	if (!head)
		return;
	printf("%%F{3}:«(%%f");
	while ((c = fgetc(head)) != EOF && c != '\n')
		if (slashes == 2)
			putchar(c);
		else if (c == '/')
			slashes++;
	printf("%%F{3})»");
	fclose(head);
}

static void cal(struct tm *t)
{
	char buf[13];
	strftime(buf, sizeof(buf), "(%a) %b %d", t);
	printf("%%F{1}󰃭 %%f%s%s  ", buf, ISORD(1) ? "st" : ISORD(2) ? "nd" :
		   ISORD(3) ? "rd" : "th");
}

static void clk(struct tm *t)
{
	printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
		   "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
		   t->tm_min);
}

static void cmdsep(struct winsize *w)
{
	unsigned short int i;
	SYMLN(w->ws_col, "%%F{1}v", "%%F{3}≥");
	printf("%%F{3}:«(");
}

static int cntdgts(int n)
{
	int i;
	for (i = !n; n; n /= 10)
		i++;
	return (i);
}

static void dirwperms(void)
{
	if (access(".", W_OK))
		printf(" %%F{5}");
}

static void disk(void)
{
	fsblkcnt_t tot;
	fsblkcnt_t rem;
	int use;
	struct statvfs s;
	statvfs("/", &s);
	tot = s.f_frsize * s.f_blocks;
	rem = s.f_frsize * s.f_bavail;
	use = ((float)(tot - rem) / tot) * 100;
	printf("%%F{%d}󰋊 %%f%d%%%%  ", use < 70 ? 2 : use < 80 ? 3 : 1, use);
	modlen_g += cntdgts(use);
}

static void exitstat(void)
{
	printf("{%%(?.%%%%.%%F{1}x)%%F{3}}⤐  ");
}

static void findgitroot(char *pwd, char **root, size_t *rootlen)
{
	size_t pwdlen = strlen(pwd);
	*root = malloc(pwdlen + 6);
	for (*rootlen = 0; *rootlen < pwdlen; ++*rootlen)
		*(*root + *rootlen) = pwd[*rootlen];
	while (1) {
		*(*root + *rootlen) = '/';
		*(*root + ++*rootlen) = '.';
		*(*root + ++*rootlen) = 'g';
		*(*root + ++*rootlen) = 'i';
		*(*root + ++*rootlen) = 't';
		*(*root + ++*rootlen) = 0;
		if (!access(*root, F_OK)) {
			*(*root + (*rootlen -= 5)) = 0;
			return;
		}
		*(*root + (*rootlen -= 5)) = 0;
		if (**root == '/' && !*(*root + 1))
			break;
		*(*root + (!(*rootlen = findrslash(*rootlen, *root)) ? ++*rootlen :
				   *rootlen)) = 0;
	}
	free(*root);
	*root = NULL;
}	

static size_t findrslash(size_t len, char *path)
{
	size_t i;
	for (i = len - 1; i; i--)
		if (path[i] == '/')
			return (i);
	return (0);
}

static void ip(void)
{
	char buf[16] = "";
	struct ifaddrs *addr;
	struct ifaddrs *list;
	getifaddrs(&list);
	for (addr = list; addr && !*buf; addr = addr->ifa_next)
		if (addr->ifa_addr && addr->ifa_addr->sa_family & AF_INET &&
			addr->ifa_flags & IFF_RUNNING && !(addr->ifa_flags & IFF_LOOPBACK))
			inet_ntop(AF_INET, &((struct sockaddr_in *)addr->ifa_addr)->sin_addr,
					  buf, sizeof(buf));
	freeifaddrs(list);
	printf("%%F{4} %%f%s  ", *buf ? buf : "127.0.0.1");
	modlen_g += strlen(buf);
}

static void modsep(struct winsize *w)
{
	unsigned short int i;
	printf("%%F{3})»:");
	SYMLN(w->ws_col - modlen_g, "%%F{1}-", "%%F{3}=");
}

static void path(char *pwd, char *gitroot, size_t gitrootlen)
{
	!gitroot || (*gitroot == '/' && !gitroot[1]) ? printf("%%F{1}%%~") :
		printf("%%F{1}@/%s", pwd + findrslash(gitrootlen, gitroot) + 1);
}

static void rootstat(void)
{
	printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void venv(void)
{
	char *path = getenv("VIRTUAL_ENV");
	if (path)
		printf("%%f(%s) ", path + findrslash(strlen(path), path) + 1);
}

int main(void)
{
	char *pwd = getenv("PWD");
	time_t tt = time(NULL);
	char *gitroot;
	size_t gitrootlen;
	struct tm *t = localtime(&tt);
	struct winsize w;
	findgitroot(pwd, &gitroot, &gitrootlen);
	ioctl(2, TIOCGWINSZ, &w);
	cmdsep(&w);
	ip();
	disk();
	bat();
	cal(t);
	clk(t);
	modsep(&w);
	rootstat();
	exitstat();
	venv();
	path(pwd, gitroot, gitrootlen);
	branch(gitroot, gitrootlen);
	dirwperms();
	printf("%%f \n");
	if (gitroot)
		free(gitroot);
	return (0);
}
