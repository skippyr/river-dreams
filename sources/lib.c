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

