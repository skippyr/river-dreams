#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIRINFO(sym, val)\
	if (val)\
		printf(" %s%d", sym, val);

typedef struct {
	int hid;
	int tmp;
	int sym;
	int exec;
	int reg;
	int dir;
	int blk;
	int ff;
	int ch;
	int soc;
} DirInfo;

static void getdirinfo(DirInfo *di);

static void
getdirinfo(DirInfo *di)
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
		if (S_ISLNK(s.st_mode))
			di->sym++;
		if (stat(e->d_name, &s))
			continue;
		if (*e->d_name == '.')
			di->hid++;
		else if (*e->d_name == '~')
			di->tmp++;
		if (S_ISDIR(s.st_mode))
			di->dir++;
		else if (S_ISBLK(s.st_mode))
			di->blk++;
		else if (S_ISFIFO(s.st_mode))
			di->ff++;
		else if (S_ISCHR(s.st_mode))
			di->ch++;
		else if (S_ISREG(s.st_mode)) {
			di->reg++;
			if (s.st_mode & S_IXUSR)
				di->exec++;
		}
		else
			di->soc++;
	}
	closedir(d);
}

int
main(void)
{
	DirInfo di;
	memset(&di, 0, sizeof(di));
	getdirinfo(&di);
	DIRINFO("%F{1} %f", di.hid);
	DIRINFO("%F{5} %f", di.tmp);
	DIRINFO("%F{2}󱖏 %f", di.exec);
	DIRINFO("%F{4}󰌷 %f", di.sym);
	DIRINFO("%F{4} %f", di.reg);
	DIRINFO("%F{3} %f", di.dir);
	DIRINFO("%F{5}🖪 %f", di.blk);
	DIRINFO("%F{2}󰔃 %f", di.ch);
	DIRINFO("%F{4}󰟦 %f", di.ff);
	DIRINFO("%F{6}󰀂 %f", di.soc);
	printf("%%(1j. %%F{5} %%f%%j.)\n");
	return 0;
}
