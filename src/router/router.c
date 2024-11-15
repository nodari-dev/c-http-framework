#include <stdio.h>
#include <stdlib.h>

#include "../../include/router/router.h"

void add_endpoint(enum HTTP_METHOD method, char *path, struct Router* r){
	r->roots->insert(r->roots, "path", "deez nuts");
}

Router* init_router(){
	Router* r = malloc(sizeof(Router));
	if(r == NULL){
		perror("Router malloc");
		exit(1);
	}
	
	r->roots = init_hashmap();
	r->add_endpoint = add_endpoint;

	return r;
}
