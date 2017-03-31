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
  if(header->method.data != WSSL_NULL)
    fprintf(file, "\"%s\"", header->method.data);
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
  if(header->uri.data != WSSL_NULL)
    fprintf(file, "\"%s\"", header->uri.data);
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
  if(header->version.data != WSSL_NULL)
    fprintf(file, "\"%s\"", header->version.data);
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
  if(wssl_header_field_chain_is_not_empty(&header->fields))
  {
    fprintf(file, "\n");
    wssl_header_field_chain_t* header_field_link;
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(header_field_link, &header->fields)
      wssl_header_field_dump(wssl_header_field_chain_entry(header_field_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}
