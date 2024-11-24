#include "./router.h"

typedef struct Serv{
	Router* router;
	char* logger_path;
} Serv;

Serv* init_service();
void start_service(Serv*);
