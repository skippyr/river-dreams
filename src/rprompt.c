#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUFSZ 1024
#define WRITEENT(clr_a, sym_a, val_a) \
	if (val_a) \
		clr_a ? printf(" %%F{%d}%s%%f%zu", clr_a, sym_a, val_a) : \
		printf("%s%zu", sym_a, val_a);

struct ents {
	size_t reg;
	size_t dir;
	size_t blk;
	size_t ch;
	size_t soc;
	size_t ff;
	size_t lnk;
};

struct linux_dirent64 {
	ino64_t d_ino;
	off64_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[];
};


static void countents(struct ents *e);
static void writeents(void);
static void writejobs(void);

static void
countents(struct ents *e)
{
	int dir = open(".", O_RDONLY);
	struct linux_dirent64 *d;
	long sz;
	long i;
	char *buf = malloc(BUFSZ);
	while ((sz = syscall(SYS_getdents64, dir, buf, BUFSZ)) > 0)
		for (i = 0; i < sz; i += d->d_reclen) {
			d = (struct linux_dirent64 *)(buf + i);
			if (*d->d_name == '.' && (!d->d_name[1] || (d->d_name[1] == '.' &&
				!d->d_name[2])))
				continue;
			switch (d->d_type) {
			case DT_REG:
				++e->reg;
				break;
			case DT_DIR:
				++e->dir;
				break;
			case DT_BLK:
				++e->blk;
				break;
			case DT_CHR:
				++e->ch;
				break;
			case DT_SOCK:
				++e->soc;
				break;
			case DT_FIFO:
				++e->ff;
				break;
			case DT_LNK:
				++e->lnk;
				break;
			}
		}
	free(buf);
	close(dir);
}

static void
writeents(void)
{
	struct ents e = {0, 0, 0, 0, 0, 0, 0};
	countents(&e);
	WRITEENT(0, " ", e.reg);
	WRITEENT(3, " ", e.dir);
	WRITEENT(5, "󰇖 ", e.blk);
	WRITEENT(2, "󱣴 ", e.ch);
	WRITEENT(6, "󱄙 ", e.soc);
	WRITEENT(4, "󰟦 ", e.ff);
	WRITEENT(4, "󰌷 ", e.lnk);
}

static void
writejobs(void)
{
	printf("%%(1j. %%F{5} %%f%%j.)");
}

int
main(void)
{
	writeents();
	writejobs();
	putchar('\n');
	return 0;
}
