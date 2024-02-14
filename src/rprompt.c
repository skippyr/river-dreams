#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DIRSTAT(clr_m, sym_m, val_m) \
	if (val_m) \
		printf(" %%F{%d}%s%%f%ld", clr_m, sym_m, val_m);

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
			if (e->d_type == DT_REG)
				d->reg++;
			else if (e->d_type == DT_DIR)
				d->dir++;
			else if (e->d_type == DT_BLK)
				d->blk++;
			else if (e->d_type == DT_CHR)
				d->ch++;
			else if (e->d_type == DT_SOCK)
				d->skt++;
			else if (e->d_type == DT_FIFO)
				d->ff++;
			else if (e->d_type == DT_LNK)
				d->lnk++;
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
	return (0);
}
