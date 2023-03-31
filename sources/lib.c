#define ENV_FALLBACK_TEXT_KEY "RIVER_DREAMS_USE_FALLBACK_TEXT"
#define ENV_COLOR_VARIANTS_KEY "RIVER_DREAMS_USE_COLOR_VARIANTS"

char *
choose_symbol_by_environment(char *default_symbol, char *fallback_symbol)
{
	return !strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "0")
	? default_symbol
	: fallback_symbol;
}

char *
choose_color_by_environment(char *default_color, char *color_variant)
{
	return !strcmp(getenv(ENV_COLOR_VARIANTS_KEY), "0")
	? default_color
	: color_variant;
}

