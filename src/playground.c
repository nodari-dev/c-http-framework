#include <stdio.h>
#include "../include/core/service.h"

char *get_deez_nuts(List* params){
	if(params != NULL){
		printf("%d\n", params->size);
		for(int i = 0; i < params->size; ++i){
			printf("%d: %s\n", i, params->arr[i]);
		}
	}
	return "GET DEEZ NUTS";
}

char *post_deez_nuts(List* params){
	if(params != NULL){
		printf("%d\n", params->size);
		for(int i = 0; i < params->size; ++i){
			printf("%d: %s\n", i, params->arr[i]);
		}
	}
	return "POST DEEZ NUTS";
}

int main(){
	WEB_Service* service = init_service();
	Router* router = init_router();
	service->router = router;
	add_endpoint(router, GET, "/deez/nuts/:id", get_deez_nuts);
	add_endpoint(router, POST, "/deez/nuts/:id", post_deez_nuts);

	// update free to actually free router
	free_router(router);
	return 0;
}
