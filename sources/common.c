#define FALLBACK_TEXT_ENVIRONMENT_VARIABLE "RIVER_DREAMS_USE_FALLBACK_TEXT"

unsigned short int is_to_use_fallback_text(void)
{
	char *fallback_text = getenv(FALLBACK_TEXT_ENVIRONMENT_VARIABLE);
	return (
		fallback_text == NULL ||
		strcmp(
			fallback_text,
			"1"
		) ?
		0 :
		1
	);
}

