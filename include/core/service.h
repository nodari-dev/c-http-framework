#include "./router.h"

typedef struct WEB_Service{
	Router* router;
	char* logger_path;
} WEB_Service;

WEB_Service* init_service();
void start_service();
