#include "main.h"

_LIBRARY_FUNCTION_
void wssl_buffer_dump
(
  _WSSL_IN_ const wssl_buffer_t* buffer,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "buffer=\n"
      INDENT_FORMAT "data: "
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  if(wssl_buffer_is_allocated(buffer))
  {
    wssl_size_t data_index;

    fprintf(file, "\"");
    for(data_index = 0; data_index < buffer->used; data_index++)
    {
      if(data_index != 0)
        fprintf(file, " ");
      fprintf(file, "%02" PRIx8, buffer->data[data_index]);
    }
    fprintf(file, "\"");
  }
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "size: %d\n"
      INDENT_FORMAT "used: %d\n"
    ),
    INDENT(indent_level+1), buffer->size,
    INDENT(indent_level+1), buffer->used
  );
}
