#include <stdio.h>
#include <stdlib.h>
#include "../include/core/service.h"

char *get_deez_nuts(List* params){
	// if(params != NULL){
	// 	printf("%d\n", params->size);
	// 	for(int i = 0; i < params->size; ++i){
	// 		printf("%d: %s\n", i, params->arr[i]);
	// 	}
	// }
	return "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: 26\r\n"
                        "\r\n"
                        "<h1>GET you suck</h1>";
}

char *post_deez_nuts(List* params){
	// if(params != NULL){
	// 	printf("%d\n", params->size);
	// 	for(int i = 0; i < params->size; ++i){
	// 		printf("%d: %s\n", i, params->arr[i]);
	// 	}
	// }
	//
	return "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: 26\r\n"
                        "\r\n"
                        "<h1>POST you suck</h1>";
}

int main(){
	Serv* service = init_service();
	add_endpoint(service->router, GET, "/foo", get_deez_nuts);
	add_endpoint(service->router, POST, "/baz", post_deez_nuts);

	// HTTP_REQUEST *r = (HTTP_REQUEST*)malloc(sizeof(HTTP_REQUEST));
	// r->uri = "/bar";
	// r->method = GET;
	// printf("%s\n", call_endpoint(service->router, r));
	start_service(service);
	// update free to actually free router
	// free_router(router);
	return 0;
}
