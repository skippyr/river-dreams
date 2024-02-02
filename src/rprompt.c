#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
		switch (e->d_type == DT_LNK && ++di->lnk && !(stat(e->d_name, &s)) ?
				s.st_mode & S_IFMT : e->d_type) {
		case S_IFDIR: case DT_DIR: di->dir++; break;
		case S_IFBLK: case DT_BLK: di->blk++; break;
		case S_IFIFO: case DT_FIFO: di->ff++; break;
		case S_IFCHR: case DT_CHR: di->ch++; break;
		case S_IFSOCK: case DT_SOCK: di->skt++; break;
		case S_IFREG: case DT_REG:
			di->reg++;
			if (!access(e->d_name, X_OK))
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
