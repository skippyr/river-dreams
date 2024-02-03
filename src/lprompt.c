#define _GNU_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#ifndef BAT
#define BAT "/sys/class/power_supply/BAT0"
#endif
#define ISORD(n) !((t->tm_mday - n) % 10)

static void bat(void);
static void branch(char *root);
static void cal(struct tm *t);
static void clk(struct tm *t);
static void cmdsep(struct winsize *w);
static int cntdgts(int n);
static void dirw(void);
static void disk(void);
static void exitcd(void);
static char *findgitroot(char *pwd);
static int findrslash(char *path);
static void ip(void);
static void modsep(struct winsize *w);
static void path(char *pwd, char *root);
static void rootusr(void);
static void venv(void);

static int modlen = 41;

static void bat(void) {
  int statfd = open(BAT "/status", O_RDONLY);
  int capfd = open(BAT "/capacity", O_RDONLY);
  int rem;
  char stat[1];
  char cap[5];
  if (statfd < 0) {
    return;
  }
  read(statfd, stat, sizeof(stat));
  read(capfd, cap, sizeof(cap));
  close(statfd);
  close(capfd);
  rem = atoi(cap);
  printf("%s%s%%f%d%%%%  ", *stat == 'C' ? "%F{3}󱐋 " : "", rem <= 5 ? "%F{1}  " :
         rem <= 25 ? "%F{3}  " : rem <= 50 ? "%F{2}  " : "%F{2}  ", rem);
  modlen += cntdgts(rem) + 6 + (*stat == 'C') * 2;
}

static void branch(char *root) {
  if (!root) {
    return;
  }
  int slashes = 0;
  int c;
  char *head;
  FILE *f;
  head = malloc(strlen(root) + 11);
  sprintf(head, "%s/.git/HEAD", root);
  f = fopen(head, "r");
  free(head);
  if (!f) {
    return;
  }
  printf("%%F{3}:«(%%f");
  while ((c = fgetc(f)) != EOF && c != '\n') {
    if (slashes == 2) {
      printf("%c", c);
    } else if (c == '/') {
      slashes++;
    }
  }
  printf("%%F{3})»");
  fclose(f);
}

static void cal(struct tm *t) {
  char buf[13];
  strftime(buf, sizeof(buf), "(%a) %b %d", t);
  printf("%%F{1}󰃭 %%f%s%s  ", buf, ISORD(1) ? "st" : ISORD(2) ? "nd" : ISORD(3) ?
         "rd" : "th");
}

static void clk(struct tm *t) {
  printf("%s%%f%02dh%02dm", t->tm_hour < 6 ? "%F{6}󰭎 " : t->tm_hour < 12 ?
         "%F{1}󰖨 " : t->tm_hour < 18 ? "%F{4} " : "%F{3}󰽥 ", t->tm_hour,
         t->tm_min);
}

static void cmdsep(struct winsize *w) {
  int i;
  for (i = 0; i < w->ws_col; i++) {
    printf(i % 2 ? "%%F{1}v" : "%%F{3}≥");
  }
  printf("%%F{3}:«(");
}

static int cntdgts(int n) {
  int i;
  for (i = !n; n; n /= 10) {
    i++;
  }
  return i;
}

static void dirw(void) {
  if (access(".", W_OK)) {
    printf(" %%F{5}");
  }
}

static void disk(void) {
  fsblkcnt_t rem;
  fsblkcnt_t tot;
  int use;
  struct statvfs s;
  statvfs("/", &s);
  tot = s.f_frsize * s.f_blocks;
  rem = s.f_frsize * s.f_bavail;
  use = ((float)(tot - rem) / tot) * 100;
  printf("%%F{%d}󰋊 %%f%d%%%%  ", use < 70 ? 2 : use < 80 ? 3 : 1, use);
  modlen += cntdgts(use);
}

static void exitcd(void) {
  printf("{%%(?.%%%%.%%F{1}x)%%F{3}}⤐  ");
}

static char *findgitroot(char *pwd) {
  char *dir = malloc(strlen(pwd) + 1);
  char *ent;
  int rslash;
  strcpy(dir, pwd);
  while (1) {
    ent = malloc(strlen(dir) + 6);
    sprintf(ent, "%s/.git", dir);
    if (!access(ent, F_OK)) {
      free(ent);
      return dir;
    }
    free(ent);
    if (!strcmp(dir, "/")) {
      free(dir);
      break;
    }
    dir[!(rslash = findrslash(dir)) ? 1 : rslash] = 0;
  }
  return NULL;
}

static int findrslash(char *path) {
  int i;
  for (i = strlen(path) - 1; i; i--) {
    if (path[i] == '/') {
      return i;
    }
  }
  return 0;
}

static void ip(void) {
  char ip[16] = "127.0.0.1";
  struct ifaddrs *addrlist;
  struct ifaddrs *addr;
  getifaddrs(&addrlist);
  for (addr = addrlist; addr; addr = addr->ifa_next) {
    if (addr->ifa_addr && addr->ifa_addr->sa_family & AF_INET &&
        addr->ifa_flags & IFF_RUNNING && !(addr->ifa_flags & IFF_LOOPBACK)) {
      inet_ntop(AF_INET, &((struct sockaddr_in *)addr->ifa_addr)->sin_addr, ip,
                sizeof(ip));
      break;
    }
  }
  freeifaddrs(addrlist);
  printf("%%F{4} %%f%s  ", ip);
  modlen += strlen(ip);
}

static void modsep(struct winsize *w) {
  int i;
  printf("%%F{3})»:");
  for (i = 0; i < w->ws_col - modlen; i++) {
    printf(i % 2 ? "%%F{1}-" : "%%F{3}=");
  }
}

static void path(char *pwd, char *root) {
  !root || !strcmp(root, "/")
    ? printf("%%F{1}%%~") : printf("%%F{1}@/%s", pwd + findrslash(root) + 1);
}

static void rootusr(void) {
  printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.)");
}

static void venv(void) {
  char *path = getenv("VIRTUAL_ENV");
  if (path) {
    printf("%%f(%s) ", path + findrslash(path) + 1);
  }
}

int main(void) {
  char *pwd = getenv("PWD");
  char *root = findgitroot(pwd);
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  struct winsize w;
  ioctl(2, TIOCGWINSZ, &w);
  cmdsep(&w);
  ip();
  disk();
  bat();
  cal(t);
  clk(t);
  modsep(&w);
  rootusr();
  exitcd();
  venv();
  path(pwd, root);
  branch(root);
  dirw();
  printf("%%f \n");
  if (root) {
    free(root);
  }
  return 0;
}
