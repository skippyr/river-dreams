#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIRINFO(sym, val)\
	if (val)\
		printf(" %s%d", sym, val);

typedef struct {
	int hid, tmp, lnk, exe, reg, dir, blk, ff, ch, skt;
} DirInfo;

static void getdirinfo(DirInfo *di);

static void getdirinfo(DirInfo *di)
{
	DIR *d = opendir(".");
	struct dirent *e;
	struct stat s;
	if (!d)
		return;
	while ((e = readdir(d))) {
		if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
			continue;
		switch (*e->d_name) {
		case '.': di->hid++; break;
		case '~': di->tmp++; break;
		}
		lstat(e->d_name, &s);
		if (S_ISLNK(s.st_mode)) {
			di->lnk++;
			if (stat(e->d_name, &s))
				continue;
		}
		switch (s.st_mode & S_IFMT) {
		case S_IFDIR: di->dir++; break;
		case S_IFBLK: di->blk++; break;
		case S_IFIFO: di->ff++; break;
		case S_IFCHR: di->ch++; break;
		case S_IFSOCK: di->skt++; break;
		case S_IFREG:
			di->reg++;
			if (s.st_mode & S_IXUSR)
				di->exe++;
			break;
		}
	}
	closedir(d);
}

int main(void)
{
	DirInfo di;
	memset(&di, 0, sizeof(di));
	getdirinfo(&di);
	DIRINFO("%F{1} %f", di.hid);
	DIRINFO("%F{5}󰚰 %f", di.tmp);
	DIRINFO("%F{2}󱖏 %f", di.exe);
	DIRINFO("%F{4}󰌷 %f", di.lnk);
	DIRINFO("%F{4} %f", di.reg);
	DIRINFO("%F{3} %f", di.dir);
	DIRINFO("%F{5}󰇖 %f", di.blk);
	DIRINFO("%F{2}󱣴 %f", di.ch);
	DIRINFO("%F{4}󰟦 %f", di.ff);
	DIRINFO("%F{6}󱄙 %f", di.skt);
	printf("%%(1j. %%F{5} %%f%%j.)\n");
	return (0);
}
