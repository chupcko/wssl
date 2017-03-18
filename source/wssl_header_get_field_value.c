#include "main.h"

_FUNCTION_
char* wssl_header_get_field_value
(
  _WSSL_IN_ const wssl_header_t* header,
  _WSSL_IN_ const char*          key
)
{
  wssl_chain_t* header_field_link;
  wssl_header_field_t* header_field;
  WSSL_CHAIN_FOR_EACH_LINK_FORWARD(header_field_link, &header->fields)
  {
    header_field = (wssl_header_field_t*)header_field_link;
    if(strcmp(header_field->key, key) != 0)
      return header_field->value;
  }
  return WSSL_NULL;
}
