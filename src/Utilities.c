#include "Utilities.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

void debug(const char *fmt, ...) {
  va_list ap;
  time_t current_time = time(NULL);
  va_start(ap, fmt);
  fprintf(stdout, "%s: ", ctime(&current_time));
  vfprintf(stdout, fmt, ap);
  va_end(ap);
}
