#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char* get_directory_path_abbreviated(
  char* directory_path,
  char* directory_path_abbreviated
) {
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
  return directory_path_abbreviated;
}

int has_ownership(char* directory_path) {
  struct stat directory_status;
  stat(directory_path, &directory_status);
  if (directory_status.st_uid == getuid()) {
    return 1;
  }
  return 0;
}

char* get_ownership_symbol() {
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
    ? " %F{red}" :
    "  %F{red}LOCKED";
}

int main() {
  char* pwd = getenv("PWD");
  char* home = getenv("HOME");
  char* directory_path = (char*) malloc(strlen(pwd));
  strcpy(directory_path, pwd);
  if (strstr(pwd, home) != NULL) {
    snprintf(
      directory_path,
      strlen(directory_path),
      "%s%s",
      "~",
      pwd + strlen(home)
    );
  }
  char* directory_path_abbreviated = (char*) malloc(strlen(directory_path));
  get_directory_path_abbreviated(directory_path, directory_path_abbreviated);
  printf(
    "%%F{green}%s%s%%f\n",
    directory_path_abbreviated,
    has_ownership(pwd) ? "" : get_ownership_symbol()
  );
  free(directory_path);
  free(directory_path_abbreviated);
  return 0;
}

