#include "main.h"

_FUNCTION_
void wssl_header_clean
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  if(header->method != WSSL_NULL)
    free((void*)header->method);
  if(header->uri != WSSL_NULL)
    free((void*)header->uri);
  if(header->method != WSSL_NULL)
    free((void*)header->version);

  wssl_header_field_chain_t* header_field_link;
  wssl_header_field_chain_t* header_field_link_next;
  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(header_field_link, header_field_link_next, &header->fields)
    wssl_header_field_delete(wssl_header_field_chain_entry(header_field_link));
}
