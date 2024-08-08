#include "common.h"
#include <string.h>
#include <tmk.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

struct DirectoryStat {
  int totalDirectories;
  int totalFiles;
  int totalHiddenEntries;
  int totalTemporaryEntries;
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
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
