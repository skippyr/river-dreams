#include "right-prompt.hpp"

static struct EntryTypesCount countEntryTypes() {
  DIR *directory = opendir(".");
  struct EntryTypesCount count;
  if (!directory) {
    return count;
  }
  for (struct dirent *entry; (entry = readdir(directory));) {
    if (!std::strcmp(entry->d_name, ".") || !std::strcmp(entry->d_name, "..")) {
      continue;
    } else if (entry->d_type == DT_REG) {
      ++count.regulars;
    } else if (entry->d_type == DT_DIR) {
      ++count.directories;
    } else if (entry->d_type == DT_BLK) {
      ++count.blocks;
    } else if (entry->d_type == DT_CHR) {
      ++count.characters;
    } else if (entry->d_type == DT_SOCK) {
      ++count.sockets;
    } else if (entry->d_type == DT_FIFO) {
      ++count.fifos;
    } else if (entry->d_type == DT_LNK) {
      ++count.symlinks;
    }
  }
  closedir(directory);
  return count;
}

static void writeEntryTypesCountModule() {
  struct EntryTypesCount count = countEntryTypes();
  if (count.regulars) {
    std::cout << " %F{blue} %f" << count.regulars;
  }
  if (count.directories) {
    std::cout << " %F{yellow} %f" << count.directories;
  }
  if (count.blocks) {
    std::cout << " %F{magenta}󰇖 %f" << count.blocks;
  }
  if (count.characters) {
    std::cout << " %F{green}󱣴 %f" << count.characters;
  }
  if (count.sockets) {
    std::cout << " %F{cyan}󱄙 %f" << count.sockets;
  }
  if (count.fifos) {
    std::cout << " %F{blue}󰟦 %f" << count.fifos;
  }
  if (count.symlinks) {
    std::cout << " %F{blue}󰌷 %f" << count.symlinks;
  }
}

static void writeJobsModule() {
  std::cout << "%(1j. %F{magenta} %f%j.)";
}

int main() {
  writeEntryTypesCountModule();
  writeJobsModule();
  std::cout << std::endl;
  return 0;
}
