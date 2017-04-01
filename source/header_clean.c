#include "main.h"

_FUNCTION_
void wssl_header_clean
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  if(wssl_string_is_allocated(&header->method))
    wssl_string_free(&header->method);
  if(wssl_string_is_allocated(&header->uri))
    wssl_string_free(&header->uri);
  if(wssl_string_is_allocated(&header->version))
    wssl_string_free(&header->version);

  wssl_header_field_chain_t* header_field_link;
  wssl_header_field_chain_t* header_field_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(header_field_link, header_field_link_next, &header->fields)
    wssl_header_field_delete(wssl_header_field_chain_get_entry_from_chain_link(header_field_link));
}
