enum LOG_TYPE{
	INFO, WARNING, ERROR
};

void log_data(enum LOG_TYPE log_type, char *message);
char* log_enum_to_string(enum LOG_TYPE log_type);
