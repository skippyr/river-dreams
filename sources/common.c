#define FALLBACK_TEXT_ENVIRONMENT_VARIABLE "RIVER_DREAMS_USE_FALLBACK_TEXT"

unsigned short int is_to_use_fallback_text(void)
{
	char *fallback_text = getenv(FALLBACK_TEXT_ENVIRONMENT_VARIABLE);
	if (fallback_text == NULL)
	{
		return (0);
	}
	return (
		!strcmp(
			fallback_text,
			"1"
		) ?
		1 :
		0
	);
}

