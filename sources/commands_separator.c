#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_even_symbol() {
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? "▲"
    : "/";
}

char* get_odd_symbol() {
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? "▼"
    : "\\";
}

int main(int argc, char* argv[]) {
  int columns;
  sscanf(argv[1], "%d", &columns);
  for (int i = 1; !(i > columns); ++i) {
    if (i % 2 == 0) {
      printf("%%F{red}%s%%f", get_even_symbol());
    } else {
      printf("%%F{yellow}%s%%f", get_odd_symbol());
    }
  }
  printf("\n");
  return 0;
}

