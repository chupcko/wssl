#include "main.h"

_LIBRARY_FUNCTION_
void wssl_header_dump
(
  _WSSL_IN_     const wssl_header_t* header,
  _WSSL_MODIFY_       FILE*          file,
  _WSSL_IN_     const wssl_size_t    indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "header(%p)=\n"
      INDENT_FORMAT "method: "
    ),
    INDENT(indent_level), (void*)header,
    INDENT(indent_level+1)
  );
  wssl_string_print(&header->method, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "uri: "
    ),
    INDENT(indent_level+1)
  );
  wssl_string_print(&header->uri, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "version: "
    ),
    INDENT(indent_level+1)
  );
  wssl_string_print(&header->version, file);
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
    CHAIN_FOR_EACH_LINK_FORWARD(header_field_link, &header->fields)
      wssl_header_field_dump(wssl_header_field_chain_get_entry_from_chain_link(header_field_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}
