#include <stdio.h>
#include "../include/router/router.h"

char *get_deez_nuts(List* params){
	if(params != NULL){
		printf("%d\n", params->size);
		for(int i = 0; i < params->size; ++i){
			printf("%d: %s\n", i, params->arr[i]);
		}
	}
	return "GET DEEZ NUTS";
}

int main(){
	Router* r = init_router();
	add_endpoint(r, GET, "/deez/nuts/:id", get_deez_nuts);
	printf("%s\n", call_endpoint(r, GET, "/deez/nuts/:id"));
	free_router(r);
	return 0;
}
