#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIRINFO(sym, val)\
	if (val)\
		printf(" %s%d", sym, val);

static void getdirinfo(int *hid, int *tmp, int *sym, int *exec);

static void
getdirinfo(int *hid, int *tmp, int *sym, int *exec)
{
	DIR *d = opendir(".");
	struct dirent *e;
	struct stat s;
	if (!d)
		return;
	while ((e = readdir(d))) {
		if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
			continue;
		lstat(e->d_name, &s);
		if (*e->d_name == '.')
			(*hid)++;
		else if (*e->d_name == '~')
			(*tmp)++;
		if (S_ISLNK(s.st_mode))
			(*sym)++;
		else if (S_ISREG(s.st_mode) && s.st_mode & S_IXUSR)
			(*exec)++;
	}
	closedir(d);
}

int
main(void)
{
	int hid = 0;
	int tmp = 0;
	int sym = 0;
	int exec = 0;
	getdirinfo(&hid, &tmp, &sym, &exec);
	DIRINFO("%F{1} %f", hid);
	DIRINFO("%F{4}󰌷 %f", sym);
	DIRINFO("%F{2}󱖏 %f", exec);
	DIRINFO("%F{5} %f", tmp);
	printf("%%(1j. %%F{5} %%f%%j.)\n");
	return 0;
}
