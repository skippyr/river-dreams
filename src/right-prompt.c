#include "right-prompt.h"

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
static int promptLength_g = 0;
#endif

static void getDirectoryStat(struct DirectoryStat *directoryStat) {
  memset(directoryStat, 0, sizeof(struct DirectoryStat));
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  WIN32_FIND_DATAW entry;
  HANDLE directoryStream = FindFirstFileW(L".\\*", &entry);
  if (directoryStream == INVALID_HANDLE_VALUE) {
    return;
  }
  do {
    if (entry.cFileName[0] == '.' &&
        (!entry.cFileName[1] ||
         (entry.cFileName[1] == '.' && !entry.cFileName[2]))) {
      continue;
    }
    DWORD attributes = GetFileAttributesW(entry.cFileName);
    if (attributes & FILE_ATTRIBUTE_HIDDEN) {
      ++directoryStat->totalHiddenEntries;
    }
    if (attributes & FILE_ATTRIBUTE_TEMPORARY) {
      ++directoryStat->totalTemporaryEntries;
    }
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
      ++directoryStat->totalDirectories;
    } else {
      ++directoryStat->totalFiles;
    }
  } while (FindNextFileW(directoryStream, &entry));
  FindClose(directoryStream);
#else
  DIR *directoryStream = opendir(".");
  if (!directoryStream) {
    return;
  }
  for (struct dirent *entry; (entry = readdir(directoryStream));) {
    if (entry->d_name[0] == '.') {
      if (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2])) {
        continue;
      }
      ++directoryStat->totalHiddenEntries;
    }
    size_t entryNameLength = strlen(entry->d_name);
    if (entry->d_name[entryNameLength - 1] == '~') {
      ++directoryStat->totalTemporaryEntries;
    }
    struct stat status;
    switch (entry->d_type == DT_LNK && ++directoryStat->totalSymlinks &&
                    !stat(entry->d_name, &status)
                ? status.st_mode & S_IFMT
                : entry->d_type) {
    case S_IFREG:
    case DT_REG:
      ++directoryStat->totalFiles;
      break;
    case S_IFDIR:
    case DT_DIR:
      ++directoryStat->totalDirectories;
      break;
    case S_IFBLK:
    case DT_BLK:
      ++directoryStat->totalBlocks;
      break;
    case S_IFCHR:
    case DT_CHR:
      ++directoryStat->totalCharacters;
      break;
    case S_IFIFO:
    case DT_FIFO:
      ++directoryStat->totalFifos;
      break;
    case S_IFSOCK:
    case DT_SOCK:
      ++directoryStat->totalSockets;
      break;
    }
  }
  closedir(directoryStream);
#endif
}

static void writeEntryStat(int isFirst, const char *symbol, int color,
                           int total) {
  if (!total) {
    return;
  }
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  promptLength_g += countDigits(total) + 2 + !isFirst;
#endif
  if (!isFirst) {
    tmk_write(" ");
  }
  if (color >= 0) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
    tmk_write("\033[3%dm%s\033[39m%d", color, symbol, total);
#else
    tmk_write("%%F{%d}%s%%f%d", color, symbol, total);
#endif
  } else {
    tmk_write("%s%d", symbol, total);
  }
}

static void writeDirectoryStat(void) {
  struct DirectoryStat stat;
  getDirectoryStat(&stat);
  writeEntryStat(1, " ", -1, stat.totalFiles);
  writeEntryStat(0, " ", 3, stat.totalDirectories);
#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
  writeEntryStat(0, "󰌷 ", 4, stat.totalSymlinks);
  writeEntryStat(0, "󰇖 ", 5, stat.totalBlocks);
  writeEntryStat(0, "󱣴 ", 2, stat.totalCharacters);
  writeEntryStat(0, "󰟦 ", 4, stat.totalFifos);
  writeEntryStat(0, "󱄙 ", 6, stat.totalSockets);
#endif
  writeEntryStat(0, "󰈉 ", 1, stat.totalHiddenEntries);
  writeEntryStat(0, "󱣹 ", 5, stat.totalTemporaryEntries);
}

int main(void) {
  writeDirectoryStat();
  tmk_writeLine("");
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /*
   * On Windows, as the right-prompt needs to be manually placed on the right
   * side using its connector, its visual length needs to be output as the
   * PowerShell .length() method of strings does not calculates it well with
   * wide-characters.
   *
   * More implementation details may be found in the respective connector.
   */
  tmk_write("%d", promptLength_g);
#endif
  return 0;
}
