#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUFSZ 1024

struct ents {
	int reg;
	int dir;
	int blk;
	int ch;
	int soc;
	int ff;
	int lnk;
	int hid;
	int tmp;
};

struct linux_dirent64 {
	ino64_t d_ino;
	off64_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[];
};

static void countents(struct ents *e);
static void writeent(int clr, char *sym, int val);
static void writeents(void);
static void writejobs(void);

static void
countents(struct ents *e)
{
	int dir = open(".", O_RDONLY);
	struct linux_dirent64 *d;
	long sz;
	long i;
	size_t j;
	char *buf = malloc(BUFSZ);
	struct stat s;
	memset(e, 0, sizeof(struct ents));
	while ((sz = syscall(SYS_getdents64, dir, buf, BUFSZ)) > 0)
		for (i = 0; i < sz; i += d->d_reclen) {
			d = (struct linux_dirent64 *)(buf + i);
			if (*d->d_name == '.') {
				if (!d->d_name[1] || (d->d_name[1] == '.' && !d->d_name[2]))
					continue;
				else
					++e->hid;
			}
			for (j = 0; d->d_name[j]; ++j);
			if (d->d_name[j - 1] == '~')
				++e->tmp;
			switch (d->d_type == DT_LNK && ++e->lnk && !stat(d->d_name, &s) ?
					s.st_mode & S_IFMT : d->d_type) {
			case S_IFREG:
			case DT_REG:
				++e->reg;
				break;
			case S_IFDIR:
			case DT_DIR:
				++e->dir;
				break;
			case S_IFBLK:
			case DT_BLK:
				++e->blk;
				break;
			case S_IFCHR:
			case DT_CHR:
				++e->ch;
				break;
			case S_IFSOCK:
			case DT_SOCK:
				++e->soc;
				break;
			case S_IFIFO:
			case DT_FIFO:
				++e->ff;
				break;
			}
		}
	free(buf);
	close(dir);
}

static void
writeent(int clr, char *sym, int val)
{
	char buf[8];
	int len;
	int i;
	int j;
	if (!val)
		return;
	clr ? printf(" %%F{%d}%s%%f", clr, sym) : printf(" %s", sym);
	sprintf(buf, "%d", val);
	for (len = 0; buf[len]; ++len);
	for (i = len - 3, j = 0; j <= len; ++j) {
		if (j == i && i)
			putchar(',');
		putchar(buf[j]);
	}
}

static void
writeents(void)
{
	struct ents e;
	countents(&e);
	writeent(0, " ", e.reg);
	writeent(3, "󰝰 ", e.dir);
	writeent(5, "󰇖 ", e.blk);
	writeent(2, "󱣴 ", e.ch);
	writeent(6, "󱄙 ", e.soc);
	writeent(4, "󰟦 ", e.ff);
	writeent(4, "󰌷 ", e.lnk);
	writeent(1, "󰈉 ", e.hid);
	writeent(5, "󱣹 ", e.tmp);
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
