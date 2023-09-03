#include <stdio.h>
#include <stdlib.h>

void
Errors__Throw_Error(char *error)
{
    fprintf(stderr, "river-dreams: %s\n", error);
    exit(EXIT_FAILURE);
}
