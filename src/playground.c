#include "../include/router/router.h"

char *get_deez_nuts(){
	return "GET DEEZ NUTS";
}

int main(){
	Router* r = init_router();
	add_endpoint(r, GET, "/deez/nuts/:id", get_deez_nuts);
	// printf("%s\n", call_endpoint(r, GET, "/deez/nuts/:id"));
	// free_router(r);
	return 0;
}
