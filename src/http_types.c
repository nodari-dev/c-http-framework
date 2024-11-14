#include "../include/http_types.h"

char* parse_http_method(enum HTTP_METHOD method){
	switch (method) {
	  case GET: return "GET";
	  case PUT: return "PUT";
	  case POST: return "POST";
	  case DELETE: return "DELETE";
	  case UNSUPPORTED: return "UNSUPPORTED";
	}
}
