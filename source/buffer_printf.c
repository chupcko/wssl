#include "main.h"

_FUNCTION_
void wssl_buffer_printf
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const char*          format,
                      ...
)
{
  va_list arguments;
  va_start(arguments, format);
  buffer->end = vsnprintf
  (
    (char*)&buffer->data[buffer->begin],
    (size_t)buffer->size,
    format,
    arguments
  );
  va_end(arguments);
}
