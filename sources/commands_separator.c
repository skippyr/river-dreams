#include <stdio.h>

int main(int argc, char* argv[]) {
  int columns;
  sscanf(argv[1], "%d", &columns);
  for (int i = 1; !(i > columns); ++i) {
    if (i % 2 == 0) {
      printf("%%F{red}%s%%f", argv[3]);
    } else {
      printf("%%F{yellow}%s%%f", argv[2]);
    }
  }
  return 0;
}

