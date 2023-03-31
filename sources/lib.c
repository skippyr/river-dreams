char *
choose_symbol_by_environment(char *default_symbol, char *fallback_symbol)
{
  return !strcmp(getenv("RIVER_DREAMS_USE_FALLBACK_TEXT"), "0")
  ? default_symbol
  : fallback_symbol;
}

char *
choose_color_by_environment(char *default_color, char *color_variant)
{
  return !strcmp(getenv("RIVER_DREAMS_USE_COLOR_VARIANTS"), "0")
  ? default_color
  : color_variant;
}

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

