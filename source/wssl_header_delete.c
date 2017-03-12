#include "main.h"

_FUNCTION_
void wssl_header_delete
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  wssl_chain_t* header_field_link;
  wssl_chain_t* header_field_link_next;
  wssl_header_field_t* header_field;

  if(header->method != WSSL_NULL)
    free((void*)header->method);
  if(header->uri != WSSL_NULL)
    free((void*)header->uri);
  if(header->method != WSSL_NULL)
    free((void*)header->version);

  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(header_field_link, header_field_link_next, &header->fields)
  {
    header_field = (wssl_header_field_t*)header_field_link;
    wssl_header_field_delete(header_field);
  }
}