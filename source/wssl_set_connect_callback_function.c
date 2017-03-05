#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_connect_callback_function
(
  _WSSL_MODIFY_ wssl_t*                           wssl,
  _WSSL_IN_     wssl_connect_callback_function_t* connect_callback_function
)
{
  wssl->connect_callback_function = connect_callback_function;
}
