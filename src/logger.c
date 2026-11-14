#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

static FILE* log_fp = NULL;
static CRITICAL_SECTION log_mutex;
static int initialized = 0;

int logger_init(const char* log_file) {
  if (initialized) return 1;

  InitializeCriticalSection(&log_mutex);
  
  if (log_fp != NULL) return 0; // already initialized.
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  if (GetConsoleMode(hOut, &dwMode)) {
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  }

  fopen_s(&log_fp, log_file, "a");
  initialized = 1;
  return (log_fp != NULL);
}

void logger_shutdown(void) {
  if (!initialized) return;
  
  if (log_fp) fclose(log_fp);
  DeleteCriticalSection(&log_mutex);
  log_fp = NULL;
}

void log_message(LogLevel level, const char* file, int line, const char* fmt, ...)
{
  if (!initialized) return;

  EnterCriticalSection(&log_mutex);
  
  const char* colors[] = { "\x1b[32m", "\x1b[33m", "\x1b[31m"};
  const char* levels[] = {"INFO", "WARN", "ERROR"};
  printf("%s", colors[level]);
  printf("[%s] (%s:%d) \x1b[0m", levels[level], file, line);

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  
  printf("\x1b[0m\n");

  if (log_fp) {
     fprintf(log_fp, "[%s] (%s:%d) ", levels[level], file, line);
     va_start(args, fmt);
     vfprintf(log_fp, fmt, args);
     va_end(args);
     fprintf(log_fp, "\n");
     fflush(log_fp);
  }

  LeaveCriticalSection(&log_mutex);
}
 
