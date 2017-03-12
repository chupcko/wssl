#include "main.h"

_LIBRARY_FUNCTION_
void wssl_header_field_dump
(
  _WSSL_IN_ const wssl_header_field_t* header_field,
  _WSSL_IN_       FILE*                file,
  _WSSL_IN_ const wssl_size_t          indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "field=\n"
      INDENT_FORMAT "key:"
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  if(header_field->key != WSSL_NULL)
    fprintf(file, "\"%s\"", header_field->key);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "value: "
    ),
    INDENT(indent_level+1)
  );
  if(header_field->value != WSSL_NULL)
    fprintf(file, "\"%s\"", header_field->value);
  else
    fprintf(file, "none");
  fprintf(file, "\n");
}
