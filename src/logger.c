#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/logger.h"

char* log_type_to_string(enum LOG_TYPE log_type) {
  switch (log_type) {
    case INFO: return "INFO";
    case ERROR: return "ERROR";
  }
}

void log_info(enum LOG_TYPE log_type, char *message) {
  FILE *file = fopen("logs.txt", "a");
  if (file == NULL) {
    perror("could not open logger file");
    exit(1);
  }

  time_t now;
  time(&now);
  char buffer[1024];
  sprintf(buffer, "%s: %s\n", ctime(&now), message);
  if (fprintf(file, "%s", buffer) < 0) {
    perror("could not write into logger file");
    exit(1);
  }

  fclose(file);
}
