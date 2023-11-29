#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>

static void
write_cmd_sep(void)
{
	struct winsize s;
	ioctl(2, TIOCGWINSZ, &s);
	for (; s.ws_col; s.ws_col--)
		printf(s.ws_col % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

static void
write_cal(struct tm *t)
{
	char *wdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},
	     *mons[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
			"Oct", "Nov", "Dec"};
	printf("%%F{1}󰸗 %%f(%s) %s %d%s", wdays[t->tm_wday], mons[t->tm_mon],
	       t->tm_mday, !((t->tm_mday - 1) % 10) ? "st" : !((t->tm_mday - 2) % 10) ?
	       "nd" : !((t->tm_mday - 3) % 10) ? "rd" : "th");
}

int
main(void)
{
	time_t e = time(NULL);
	struct tm t;
	localtime_r(&e, &t);
	write_cmd_sep();
	write_cal(&t);
	printf("%%F{3}⤐  %%F{1}%%1~%%f ");
	return 0;
}
