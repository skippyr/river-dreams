#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIRINFO(sym, val)\
	if (val)\
		printf(" %s%d", sym, val);

typedef struct {
	int hid, tmp, sym, exec, reg, dir, blk, ff, ch, soc;
} DirInfo;

static void getdirinfo(DirInfo *di);

static void
getdirinfo(DirInfo *di)
{
	DIR *d = opendir(".");
	struct dirent *e;
	struct stat s;
	int len;
	if (!d)
		return;
	while ((e = readdir(d))) {
		len = strlen(e->d_name);
		if ((len == 1 && *e->d_name == '.') ||
		    (len == 2 && *e->d_name == '.' && e->d_name[1] == '.'))
			continue;
		lstat(e->d_name, &s);
		if (S_ISLNK(s.st_mode))
			di->sym++;
		if (stat(e->d_name, &s))
			continue;
		switch (*e->d_name) {
			case '.': di->hid++; break;
			case '~': di->tmp++; break;
		}
		switch (s.st_mode & S_IFMT) {
			case S_IFDIR: di->dir++; break;
			case S_IFBLK: di->blk++; break;
			case S_IFIFO: di->ff++; break;
			case S_IFCHR: di->ch++; break;
			case S_IFSOCK: di->soc++; break;
			case S_IFREG:
				di->reg++;
				if (s.st_mode & S_IXUSR)
					di->exec++;
				break;
		}
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
