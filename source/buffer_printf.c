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
  int length = vsnprintf
  (
    (char*)&buffer->data[buffer->begin],
    (size_t)(buffer->size-buffer->begin),
    format,
    arguments
  );
  va_end(arguments);

  if(length < 0)
    buffer->end = buffer->begin;
  else
  {
    buffer->end = buffer->begin+(wssl_size_t)length;
    if(buffer->end > buffer->size)
      buffer->end = buffer->size;
  }
}
