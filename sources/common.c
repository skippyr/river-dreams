#define FALLBACK_TEXT_ENVIRONMENT_VARIABLE "RIVER_DREAMS_USE_FALLBACK_TEXT"

unsigned short int
is_to_use_fallback_text(void)
{
	return (
		!strcmp(getenv(FALLBACK_TEXT_ENVIRONMENT_VARIABLE), "1")
		? 1
		: 0
	);
}

