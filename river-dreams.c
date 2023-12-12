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

static void cmdsep(struct winsize *w);

static int modlen = 41;

static void
cmdsep(struct winsize *w)
{
	int i;
	for (i = 0; i < w->ws_col; i++)
		printf(i % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

int
main(void)
{
	time_t rt = time(NULL);
	struct winsize w;
	struct tm t;
	localtime_r(&rt, &t);
	ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
	cmdsep(&w);
	return 0;
}
