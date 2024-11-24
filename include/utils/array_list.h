#ifndef _ARRLIST
#define _ARRLIST

typedef struct List{
	char** arr;
	unsigned int size;
	int last_index;
} List;

List* init_list();
void push(struct List*, char*);
void free_list(struct List*);

#endif
