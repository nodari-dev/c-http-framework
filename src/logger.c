#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/logger.h"

char* log_enum_to_string(enum LOG_TYPE log_type) {
  switch (log_type) {
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
  }
}

void log_data(enum LOG_TYPE log_type, char *message) {
  FILE *file = fopen("logs.txt", "a");
  if (file == NULL) {
    perror("could not open logger file");
    exit(1);
  }


  time_t raw_time;
  struct tm *time_info;
  char time_buffer[30];
  time(&raw_time);
  time_info = gmtime(&raw_time);
  strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", time_info);

  char* log_type_str = log_enum_to_string(log_type); 

  char buffer[1024];
  sprintf(buffer, "%s: %s %s\n", time_buffer, log_type_str, message);
  if (fprintf(file, "%s", buffer) < 0) {
    perror("could not write into logger file");
    exit(1);
  }
  printf("%s\n", buffer);

  fclose(file);
}
