#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include <dirent.h>

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

static void countents(struct ents *e);
static void writeents(void);
static void writejobs(void);

static void
countents(struct ents *e)
{
	DIR *d = opendir(".");
	struct dirent *ed;
	memset(e, 0, sizeof(struct ents));
	if (!d)
		return;
	while ((ed = readdir(d)))
		if (!(*ed->d_name == '.' && (!ed->d_name[1] || (ed->d_name[1] == '.' &&
			!ed->d_name[2])))) {
			if (ed->d_type == DT_REG)
				++e->reg;
			else if (ed->d_type == DT_DIR)
				++e->dir;
			else if (ed->d_type == DT_BLK)
				++e->blk;
			else if (ed->d_type == DT_CHR)
				++e->ch;
			else if (ed->d_type == DT_SOCK)
				++e->soc;
			else if (ed->d_type == DT_FIFO)
				++e->ff;
			else if (ed->d_type == DT_LNK)
				++e->lnk;
		}
	closedir(d);
}

static void
writeents(void)
{
	struct ents e;
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
	return 0;
}
