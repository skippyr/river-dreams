#include "right-prompt.h"

static void countEntryTypes(struct EntryTypesCount* count) {
  memset(count, 0, sizeof(struct EntryTypesCount));
  DIR* directory = opendir(".");
  if (!directory) {
    return;
  }
  for (struct dirent* entry; (entry = readdir(directory));) {
    if (*entry->d_name == '.' &&
        (!entry->d_name[1] || (entry->d_name[1] == '.' && !entry->d_name[2]))) {
      continue;
    }
    switch (entry->d_type) {
    case DT_REG:
      ++count->regulars;
      break;
    case DT_DIR:
      ++count->directories;
      break;
    case DT_BLK:
      ++count->blocks;
      break;
    case DT_CHR:
      ++count->characters;
      break;
    case DT_SOCK:
      ++count->sockets;
      break;
    case DT_FIFO:
      ++count->fifos;
      break;
    case DT_LNK:
      ++count->symlinks;
      break;
    }
  }
  closedir(directory);
}

static void writeEntryTypesCountModule(void) {
  struct EntryTypesCount count;
  countEntryTypes(&count);
  WRITE_ENTRY_TYPE_COUNT((char*)NULL, " ", count.regulars);
  WRITE_ENTRY_TYPE_COUNT("yellow", " ", count.directories);
  WRITE_ENTRY_TYPE_COUNT("magenta", "󰇖 ", count.blocks);
  WRITE_ENTRY_TYPE_COUNT("green", "󱣴 ", count.characters);
  WRITE_ENTRY_TYPE_COUNT("cyan", "󱄙 ", count.sockets);
  WRITE_ENTRY_TYPE_COUNT("blue", "󰟦 ", count.fifos);
  WRITE_ENTRY_TYPE_COUNT("blue", "󰌷 ", count.symlinks);
}

static void writeJobsCountModule(void) {
  printf("%%(1j. %%F{magenta} %%f%%j.)");
}

int main(void) {
  writeEntryTypesCountModule();
  writeJobsCountModule();
  putchar('\n');
  return 0;
}
