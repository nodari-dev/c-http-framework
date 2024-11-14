#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _STRINGBUILDER
#define __STRINGBUILDER

typedef struct {
	size_t capacity;
	size_t len;
	char* buf;
} String_Builder;

String_Builder* init_string_builder();
void append_chars(String_Builder* sb, char* chars);
void free_string_builder(String_Builder* sb);

#endif
