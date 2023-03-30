#include "../utils.c"

int
main()
{
  printf(
    "%%(?..%%F{yellow}[%%F{red}%s%%B%%?%%b%%F{yellow}]%%f)%%(!.%%F{yellow}\
[%%F{red}#%%F{yellow}].)%%(?.%%F{yellow}.%%F{red})%s%%f\n",
    choose_symbol(" ", "X "),
    choose_symbol("⤐  ", "> ")
  );
  return 0;
}

