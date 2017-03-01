#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_global_extra_data
(
  _WSSL_MODIFY_ wssl_t* wssl,
  _WSSL_IN_     void*   global_extra_data
)
{
  wssl->global_extra_data = global_extra_data;
}
