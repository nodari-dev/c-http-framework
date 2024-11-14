#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/utils/string_builder.h"

#define STR_BUILDER_MIN_CAP 128

void extend_capacity_my_ass(String_Builder* sb, size_t new_size){
	while(sb->capacity < new_size){
		sb->capacity *= 2;
	}

	char* new_buffer = (char*)realloc(sb->buf, sb->capacity);
	if(new_buffer == NULL){
		perror("String_Builder buffer extencion realloc");
		exit(1);
	}
	sb->buf = new_buffer;
}

void free_string_builder(String_Builder* sb){
	free(sb->buf);
	free(sb);
}

String_Builder* init_string_builder(){
	String_Builder* sb = (String_Builder*)malloc(sizeof(String_Builder));
	if(sb == NULL){
		return NULL;
	}

	sb->capacity = STR_BUILDER_MIN_CAP;
	sb->len = 0;

	sb->buf = (char*)malloc(sb->capacity);

	if(sb->buf == NULL){
		perror("String_Builder buffer init");
		exit(1);
	}
	sb->buf[0] = '\0';

	return sb;
}


void append_chars(String_Builder* sb, char* chars){
	size_t string_size = strlen(chars);

	if(sb->len + string_size > sb->capacity){
		extend_capacity_my_ass(sb, string_size + sb->len + 1);
	}

	strcat(sb->buf, chars);
	sb->len += string_size;
}

void resrart_string_builder(String_Builder* sb){
	free(sb->buf);
	sb->buf = NULL;
	sb->capacity = STR_BUILDER_MIN_CAP;
	sb->len = 0;
}

