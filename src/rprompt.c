#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define DIRINFO(sym, val)\
  if (val) {\
    printf(" %s%d", sym, val);\
  }

struct dirinfo {
  int reg;
  int dir;
  int blk;
  int ch;
  int skt;
  int ff;
  int lnk;
};

struct linux_dirent64 {
  ino64_t d_ino;
  off64_t d_off;
  unsigned short d_reclen;
  unsigned char  d_type;
  char d_name[];
};

static void getdirinfo(struct dirinfo *di) {
  char buf[1024];
  int fd = open(".", O_RDONLY);
  int ents;
  int i;
  struct linux_dirent64 *e;
  if (fd < 0) {
    return;
  }
  while ((ents = syscall(SYS_getdents64, fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < ents; i += e->d_reclen) {
      e = (struct linux_dirent64 *)(buf + i);
      if (e->d_name[0] == '.' && (!e->d_name[1] || (e->d_name[1] == '.' && !e->d_name[2]))) {
        continue;
      }
      switch (e->d_type) {
      case DT_REG: di->reg++; break;
      case DT_DIR: di->dir++; break;
      case DT_BLK: di->blk++; break;
      case DT_CHR: di->ch++; break;
      case DT_SOCK: di->skt++; break;
      case DT_FIFO: di->ff++; break;
      case DT_LNK: di->lnk++; break;
      }
    }
  }
  close(fd);
}

int main(void) {
  struct dirinfo di = {0, 0, 0, 0, 0, 0, 0};
  getdirinfo(&di);
  DIRINFO("%F{4} %f", di.reg);
  DIRINFO("%F{3} %f", di.dir);
  DIRINFO("%F{5}󰇖 %f", di.blk);
  DIRINFO("%F{2}󱣴 %f", di.ch);
  DIRINFO("%F{4}󰟦 %f", di.ff);
  DIRINFO("%F{6}󱄙 %f", di.skt);
  DIRINFO("%F{4}󰌷 %f", di.lnk);
  printf("%%(1j. %%F{5} %%f%%j.)\n");
  return 0;
}
