#include "../../include/http/http_reponses.h"

char *gen_http_resp(enum HTTP_STATUS_CODES code, char *body) {
  // TODO: CALCULATE CONTENT LEN
  if (code == NOT_FOUND) {
    return "HTTP/1.1 404 Not Found\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: 26\r\n"
           "\r\n"
           "<h1>you bro, what are you looking for?</h1>";
  }

  return "";
}
