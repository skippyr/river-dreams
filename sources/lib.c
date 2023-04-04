#define ENV_FALLBACK_TEXT_KEY "RIVER_DREAMS_USE_FALLBACK_TEXT"
#define ENV_COLOR_VARIANTS_KEY "RIVER_DREAMS_USE_COLOR_VARIANTS"

const char *
choose_symbol_by_environment(const char *default_symbol, const char *fallback_symbol)
{
	return !strcmp(getenv(ENV_FALLBACK_TEXT_KEY), "1")
	? fallback_symbol
	: default_symbol;
}

