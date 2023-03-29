#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char* pwd = getenv("PWD");
  char* home = getenv("HOME");
  char* directory_path = pwd;
  if (strstr(pwd, home) != NULL) {
    snprintf(
      directory_path,
      strlen(directory_path),
      "%s%s",
      "~",
      pwd + strlen(home)
    );
  }
  int path_slice_last_index = 0;
  for (int i = 0; i != strlen(directory_path); ++i) {
    if (directory_path[i] == '/' && i != 0) {
      ++path_slice_last_index;
    }
  }
  char* directory_path_abbreviated = (char*) malloc(strlen(directory_path));
  int directory_path_abbreviated_index = 0;
  char last_character;
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
      *(directory_path_abbreviated + directory_path_abbreviated_index) = directory_path[i];
      ++directory_path_abbreviated_index;
    } else {
      if (directory_path[i - 1] == '/') {
        *(directory_path_abbreviated + directory_path_abbreviated_index) = directory_path[i];
        ++directory_path_abbreviated_index;
      } else if (directory_path[i - 2] == '/' && directory_path[i - 1] == '.') {
        *(directory_path_abbreviated + directory_path_abbreviated_index) = directory_path[i];
        ++directory_path_abbreviated_index;
      }
    }
    last_character = directory_path[i];
  }
  printf("%s\n", directory_path_abbreviated);
  free(directory_path_abbreviated);
  return 0;
}

