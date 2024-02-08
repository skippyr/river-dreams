#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DIRSTAT(sym, val)\
	if (val)\
		printf(" %s%ld", sym, val);

struct dirstat
{
	size_t reg;
	size_t dir;
	size_t blk;
	size_t ch;
	size_t skt;
	size_t ff;
	size_t lnk;
};

struct linux_dirent64
{
	ino64_t d_ino;
	off64_t d_off;
	unsigned short d_reclen;
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
	DIRSTAT("%F{4} %f", d.reg);
	DIRSTAT("%F{3} %f", d.dir);
	DIRSTAT("%F{5}󰇖 %f", d.blk);
	DIRSTAT("%F{2}󱣴 %f", d.ch);
	DIRSTAT("%F{4}󰟦 %f", d.ff);
	DIRSTAT("%F{6}󱄙 %f", d.skt);
	DIRSTAT("%F{4}󰌷 %f", d.lnk);
	printf("%%(1j. %%F{5} %%f%%j.)\n");
	return 0;
}
