#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_local_extra_data
(
  _WSSL_MODIFY_ wssl_connection_info_t* connection_info,
  _WSSL_IN_     void*                   local_extra_data
)
{
  connection_info->local_extra_data = local_extra_data;
}
