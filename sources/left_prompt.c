#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include "lib.c"

int
has_ownership(char* path)
{
  unsigned int user_uid = getuid();
  struct stat status;
  stat(path, &status);
  if (status.st_uid == user_uid || user_uid == 0) {
    return 1;
  }
  return 0;
}

void
get_directory_path_abbreviated(
  char *directory_path,
  char *directory_path_abbreviated
)
{
  int path_slice_last_index = 0;
  for (int i = 0; i != strlen(directory_path); ++i) {
    if (directory_path[i] == '/' && i != 0) {
      ++path_slice_last_index;
    }
  }
  int directory_path_abbreviated_index = 0;
  int path_slice_index = 0;
  for (int i = 0; i != strlen(directory_path); ++i) {
    if (directory_path[i] == '/' && i != 0) {
      ++path_slice_index;
    }
    if (
      path_slice_index == path_slice_last_index ||
      directory_path[i] == '/' ||
      directory_path[i] == '.' ||
      directory_path[i] == '~'
    ) {
      *(directory_path_abbreviated + directory_path_abbreviated_index) =
        directory_path[i];
      ++directory_path_abbreviated_index;
    } else {
      if (directory_path[i - 1] == '/') {
        *(directory_path_abbreviated + directory_path_abbreviated_index) =
          directory_path[i];
        ++directory_path_abbreviated_index;
      } else if (directory_path[i - 2] == '/' && directory_path[i - 1] == '.') {
        *(directory_path_abbreviated + directory_path_abbreviated_index) =
          directory_path[i];
        ++directory_path_abbreviated_index;
      }
    }
  }
}

void
print_separator()
{
  struct winsize terminal_size;
  ioctl(0, TIOCGWINSZ, &terminal_size);

  for (int i = 1; !(i > terminal_size.ws_col); ++i) {
    if (i % 2 == 0) {
      printf("%%F{red}%s%%f", choose_symbol_by_environment("▲", "/"));
    } else {
      printf(
        "%%F{%s}%s%%f",
        choose_color_by_environment("yellow", "blue"),
        choose_symbol_by_environment("▼", "\\")
      );
    }
  }
}

void
print_top_connector_left()
{
  printf("%%F{red}┌─%%F{yellow}[%%f");
}

void
print_disk_usage_percentage()
{
  struct statvfs sysdisk_status;
  statvfs("/", &sysdisk_status);
  unsigned long total = sysdisk_status.f_blocks * sysdisk_status.f_bsize;
  unsigned long free = sysdisk_status.f_bfree * sysdisk_status.f_bsize;
  unsigned long used = total - free;
  unsigned int percentage = (unsigned int) (((float) used / total) * 100);
  printf(
    "%%F{green}%s%%f%u%%%%",
    choose_symbol_by_environment(" ", "DISK "),
    percentage
  );
}

void
print_pyenv()
{
  char *pyenv = getenv("VIRTUAL_ENV");
  if (pyenv != NULL) {
    printf(
      " %%F{red}%s%%f%s",
      choose_symbol_by_environment("󰚐 ", "PYENV "),
      basename(pyenv)
    );
  }
}

void
print_top_connector_right()
{
  printf("%%F{yellow}]%%f\n");
}

void
print_bottom_connector()
{
  printf("%%F{red}└%%f");
}

void
print_arrow()
{
  printf(
    "%%(?..%%F{yellow}[%%F{red}%s%%B%%?%%b%%F{yellow}]%%f)%%(!.%%F{yellow}\
[%%F{red}#%%F{yellow}].)%%(?.%%F{yellow}.%%F{red})%s%%f",
    choose_symbol_by_environment(" ", "X "),
    choose_symbol_by_environment("⤐  ", "> ")
  );
}

void
print_directory()
{
  char *pwd = getenv("PWD");
  char *home = getenv("HOME");
  char *directory_path_with_aliases = malloc(strlen(pwd));
  strcpy(directory_path_with_aliases, pwd);
  if (strstr(pwd, home) != NULL) {
    snprintf(
      directory_path_with_aliases,
      strlen(directory_path_with_aliases),
      "%s%%F{red}%s%%f",
      "~",
      pwd + strlen(home)
    );
  }
  char *directory_path_abbreviated = malloc(
    strlen(directory_path_with_aliases)
  );
  get_directory_path_abbreviated(
    directory_path_with_aliases,
    directory_path_abbreviated
  );
  printf(
    "%%F{green}%s%%F{red}%s%%f",
    directory_path_abbreviated,
    has_ownership(pwd) ? "" : choose_symbol_by_environment(" ", " LOCKED")
  );
  free(directory_path_with_aliases);
  free(directory_path_abbreviated);
}

int
main()
{
  print_separator();
  print_top_connector_left();
  print_disk_usage_percentage();
  print_pyenv();
  print_top_connector_right();
  print_bottom_connector();
  print_arrow();
  print_directory();
  return 0;
}

