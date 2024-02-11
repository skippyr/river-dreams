#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DIRSTAT(clr_m, sym_m, val_m)\
	if (val_m)\
		printf(" %%F{%d}%s%%f%d", clr_m, sym_m, val_m);

struct dirstat
{
	int reg;
	int dir;
	int blk;
	int ch;
	int skt;
	int ff;
	int lnk;
};

struct linux_dirent64
{
	ino64_t d_ino;
	off64_t d_off;
	unsigned short int d_reclen;
	unsigned char d_type;
	char d_name[];
};

static void getdirstat(struct dirstat *d)
{
	char buf[1024];
	int fd = open(".", O_RDONLY);
	long nents;
	long i;
	struct linux_dirent64 *e;
	if (fd < 0)
		return;
	while ((nents = syscall(SYS_getdents64, fd, buf, sizeof(buf))) > 0) {
		for (i = 0; i < nents; i += e->d_reclen) {
			e = (struct linux_dirent64 *)(buf + i);
			if (*e->d_name == '.' && (!e->d_name[1] || (e->d_name[1] == '.' &&
														!e->d_name[2])))
				continue;
			switch (e->d_type) {
			case DT_REG: d->reg++; break;
			case DT_DIR: d->dir++; break;
			case DT_BLK: d->blk++; break;
			case DT_CHR: d->ch++; break;
			case DT_SOCK: d->skt++; break;
			case DT_FIFO: d->ff++; break;
			case DT_LNK: d->lnk++; break;
			}
		}
	}
	close(fd);
}

int main(void)
{
	struct dirstat d = {0, 0, 0, 0, 0, 0, 0};
	getdirstat(&d);
	DIRSTAT(4, " ", d.reg);
	DIRSTAT(3, " ", d.dir);
	DIRSTAT(5, "󰇖 ", d.blk);
	DIRSTAT(2, "󱣴 ", d.ch);
	DIRSTAT(4, "󰟦 ", d.ff);
	DIRSTAT(6, "󱄙 ", d.skt);
	DIRSTAT(4, "󰌷 ", d.lnk);
	printf("%%(1j. %%F{5} %%f%%j.)\n");
	return 0;
}
