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
    fprintf(file, "\"");
    wssl_size_t data_index;
    for(data_index = buffer->begin; data_index < buffer->end; data_index++)
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
      INDENT_FORMAT "size: %zu\n"
      INDENT_FORMAT "begin: %zu\n"
      INDENT_FORMAT "end: %zu\n"
    ),
    INDENT(indent_level+1), buffer->size,
    INDENT(indent_level+1), buffer->begin,
    INDENT(indent_level+1), buffer->end
  );
}
