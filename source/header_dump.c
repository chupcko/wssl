#include "main.h"

_LIBRARY_FUNCTION_
void wssl_header_dump
(
  _WSSL_IN_ const wssl_header_t* header,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "header=\n"
      INDENT_FORMAT "method: "
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  if(header->method != WSSL_NULL)
    fprintf(file, "\"%s\"", header->method);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "uri: "
    ),
    INDENT(indent_level+1)
  );
  if(header->uri != WSSL_NULL)
    fprintf(file, "\"%s\"", header->uri);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "version: "
    ),
    INDENT(indent_level+1)
  );
  if(header->version != WSSL_NULL)
    fprintf(file, "\"%s\"", header->version);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "fields:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_chain_is_not_empty(&header->fields))
  {
    wssl_chain_t* header_field_link;

    fprintf(file, "\n");
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(header_field_link, &header->fields)
      wssl_header_field_dump((wssl_header_field_t*)header_field_link, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}
