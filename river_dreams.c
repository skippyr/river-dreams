#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

static void
write_cmd_sep(void)
{
	struct winsize s;
	ioctl(2, TIOCGWINSZ, &s);
	for (; s.ws_col; s.ws_col--)
		printf(s.ws_col % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
}

int
main(void)
{
	write_cmd_sep();
	printf("%%F{3}⤐  %%F{1}%%1~%%f ");
	return 0;
}
