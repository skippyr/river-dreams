#define ENV_FALLBACK_TEXT_KEY "RIVER_DREAMS_USE_FALLBACK_TEXT"
#define ENV_COLOR_VARIANTS_KEY "RIVER_DREAMS_USE_COLOR_VARIANTS"

const char *
choose_symbol_by_environment(const char *default_symbol, const char *fallback_symbol)
{
	return !strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "0")
	? default_symbol
	: fallback_symbol;
}

const char *
choose_color_by_environment(const char *default_color, const char *color_variant)
{
	return !strcmp(getenv(ENV_COLOR_VARIANTS_KEY), "0")
	? default_color
	: color_variant;
}

