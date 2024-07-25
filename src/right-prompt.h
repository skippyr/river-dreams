#if defined(_WIN32)
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "common.h"
#include <string.h>
#include <tmk.h>

struct DirectoryStat {
  int totalDirectories;
  int totalFiles;
  int totalHiddenEntries;
  int totalTemporaryEntries;
#if !defined(_WIN32)
  int totalBlocks;
  int totalCharacters;
  int totalFifos;
  int totalSockets;
  int totalSymlinks;
#endif
};

static void getDirectoryStat(struct DirectoryStat *stat);
static void writeEntryStat(bool isFirst, const char *symbol, int color, int total);
static void writeDirectoryStat(void);
