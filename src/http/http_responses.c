#include "../../include/http/http_reponses.h"

char *gen_http_resp(enum HTTP_STATUS_CODES code, char *body) {
  // TODO: CALCULATE CONTENT LEN
  if (code == NOT_FOUND) {
	  return "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "404 Not Found";
  }

  return "";
}
