#include "../utils.c"

int
main()
{
  char *pyenv = getenv("VIRTUAL_ENV");
  if (pyenv == NULL) return 0;
  printf("%%F{red}%s%%f%s\n", choose_symbol("󰚐 ", "PYENV "), basename(pyenv));
  return 0;
}
