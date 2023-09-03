#include <stdlib.h>
#include <string.h>

#include "errors.h"

char *
Heap_Memory__Allocate_String(char *string)
{
    size_t size       = strlen(string) + 1;
    char  *allocation = malloc(size);
    if (!allocation)
    {
        Errors__Throw_Error("can't allocate enough memory.");
    }
    memcpy(allocation, string, size);
    return (allocation);
}

char *
Heap_Memory__Append_Strings(char *first_string, char *second_string)
{
    size_t first_string_size  = strlen(first_string),
           second_string_size = strlen(second_string);
    char buffer[first_string_size + second_string_size + 1];
    memcpy(buffer, first_string, first_string_size);
    memcpy(buffer + first_string_size, second_string, second_string_size);
    buffer[first_string_size + second_string_size] = 0;
    return (Heap_Memory__Allocate_String(buffer));
}

void
Heap_Memory__Deallocate(void *allocation)
{
    if (allocation)
    {
        free(allocation);
    }
}
