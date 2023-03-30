#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libgen.h>

unsigned long
to_gigabytes(unsigned long bytes)
{
  return (unsigned long) (bytes / pow(10, 9));
}

char*
choose_symbol(char* default_symbol, char* fallback_symbol)
{
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? default_symbol
    : fallback_symbol;
}

char*
choose_color(char* default_color, char* color_variant)
{
  return !strcmp(getenv("RIVER_DREAMS_USE_COLOR_VARIANTS"), "0")
    ? default_color
    : color_variant;
}
