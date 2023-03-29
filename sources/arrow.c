#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_symbol() {
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? "⤐  " 
    : "> ";
}

int main() {
  printf("%%(?.%%F{yellow}.%%F{red})%s%%f\n", get_symbol());
  return 0;
}

