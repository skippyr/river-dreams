#include "../utils.c"

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

int
has_ownership(char* directory_path)
{
  unsigned int user_uid = getuid();
  struct stat directory_status;
  stat(directory_path, &directory_status);
  if (directory_status.st_uid == user_uid || user_uid == 0) {
    return 1;
  }
  return 0;
}

int
main()
{
  char* pwd = getenv("PWD");
  char* home = getenv("HOME");
  char* directory_path_with_aliases = (char*) malloc(strlen(pwd));
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
  char* directory_path_abbreviated = (char*) malloc(
    strlen(directory_path_with_aliases)
  );
  get_directory_path_abbreviated(
    directory_path_with_aliases,
    directory_path_abbreviated
  );
  printf(
    "%%F{green}%s%%F{red}%s%%f\n",
    directory_path_abbreviated,
    has_ownership(pwd) ? "" : choose_symbol(" ", " LOCKED")
  );
  free(directory_path_with_aliases);
  free(directory_path_abbreviated);
  return 0;
}

