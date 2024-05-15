#include <cstring>
#include <iostream>
#include <memory>

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

struct EntriesCount {
  int totalBlocks = 0;
  int totalCharacters = 0;
  int totalDirectories = 0;
  int totalFifos = 0;
  int totalFiles = 0;
  int totalHiddenEntries = 0;
  int totalSockets = 0;
  int totalSymlinks = 0;
  int totalTemporaryEntries = 0;
};

struct linux_dirent64 {
  ino64_t d_ino;
  off64_t d_off;
  unsigned short d_reclen;
  unsigned char d_type;
  char d_name[];
};

static struct EntriesCount countEntriesInCWD();
static void writeEntriesCountModule();
static void writeEntryCount(const char *symbol, const char *color, int count);
static void writeJobsModule();
