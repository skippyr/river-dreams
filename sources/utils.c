#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char* choose_symbol(char* default_symbol, char* fallback_symbol) {
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? default_symbol
    : fallback_symbol;
}
