#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
} LogLevel;

int logger_init(const char* log_file);
void logger_shutdown(void);

void log_message(LogLevel level, const char* file, int line, const char* fmt, ...);

#define LOG_I(...) log_message(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_W(...) log_message(LOG_WARN, __FILE__, __LINE__,__VA_ARGS__)
#define LOG_E(...) log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif
