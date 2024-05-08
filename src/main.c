#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/core.h"
#include "../include/errors.h"
#include "../include/conf.h"

int main() {
  struct sockaddr_in host_address;
  host_address.sin_family = AF_INET;
  host_address.sin_port = htons(PORT);
  host_address.sin_addr.s_addr = inet_addr(DEFAULT_HOST);

  start_server(&host_address);

  return 0;
}
