#include "main.h"

_LIBRARY_FUNCTION_
void wssl_header_field_dump
(
  _WSSL_IN_     const wssl_header_field_t* header_field,
  _WSSL_MODIFY_       FILE*                file,
  _WSSL_IN_     const wssl_size_t          indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "field=\n"
      INDENT_FORMAT "key: "
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  wssl_string_print(&header_field->key, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "value: "
    ),
    INDENT(indent_level+1)
  );
  wssl_string_print(&header_field->value, file);
  fprintf(file, "\n");
}
