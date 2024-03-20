#include <dirent.h>

#include <cstring>
#include <iostream>

struct EntryTypesCount {
  std::size_t regulars = 0;
  std::size_t directories = 0;
  std::size_t blocks = 0;
  std::size_t characters = 0;
  std::size_t sockets = 0;
  std::size_t fifos = 0;
  std::size_t symlinks = 0;
};

static struct EntryTypesCount countEntryTypes();
static void writeEntryTypesCountModule();
static void writeJobsModule();
