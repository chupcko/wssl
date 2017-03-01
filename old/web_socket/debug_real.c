#include "main.h"

void debug_real(char* format, ...)
{
  va_list arguments;

  fputs("DEBUG: ", stderr);
  va_start(arguments, format);
  vfprintf(stderr, format, arguments);
  va_end(arguments);
  fputc('\n', stderr);
}
