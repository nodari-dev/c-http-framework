#include "../include/utils/string_builder.h"

int main(){
	String_Builder* sb = init_string_builder();
	append_chars(sb, "got ");
	append_chars(sb, "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum. MY ASS");
	printf("%zu\n", sb->len);
	printf("%s\n", sb->buf);
	free_string_builder(sb);
	return 0;
}
