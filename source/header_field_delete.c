#include "main.h"

_FUNCTION_
void wssl_header_field_delete
(
  _WSSL_MODIFY_ wssl_header_field_t* header_field
)
{
  if(header_field->value.data != WSSL_NULL)
    free((void*)header_field->value.data);

  wssl_header_field_chain_delete_link(&header_field->chain_link);

  free((void*)header_field);
}
