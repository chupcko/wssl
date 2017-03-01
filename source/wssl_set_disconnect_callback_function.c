#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_disconnect_callback_function
(
  _WSSL_MODIFY_       wssl_t*                              wssl,
  _WSSL_IN_     const wssl_disconnect_callback_function_t* disconnect_callback_function
)
{
  wssl->disconnect_callback_function = disconnect_callback_function;
}
