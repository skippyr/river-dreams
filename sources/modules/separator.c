#include "../utils.c"

int
main(int argc, char *argv[])
{
  int columns;
  sscanf(argv[1], "%d", &columns);
  for (int i = 1; !(i > columns); ++i) {
    if (i % 2 == 0) {
      printf("%%F{red}%s%%f", choose_symbol("▲", "/"));
    } else {
      printf(
        "%%F{%s}%s%%f",
        choose_color("yellow", "blue"),
        choose_symbol("▼", "\\")
      );
    }
  }
  printf("\n");
  return 0;
}

