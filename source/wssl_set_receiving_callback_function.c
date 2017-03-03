#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_receiving_callback_function
(
  _WSSL_MODIFY_       wssl_t*                             wssl,
  _WSSL_IN_     const wssl_receiving_callback_function_t* receiving_callback_function
)
{
  wssl->receiving_callback_function = receiving_callback_function;
}
