#include "main.h"

_LIBRARY_FUNCTION_
void wssl_set_tick_callback_function
(
  _WSSL_MODIFY_ wssl_t*                        wssl,
  _WSSL_IN_     wssl_tick_callback_function_t* tick_callback_function
)
{
  wssl->tick_callback_function = tick_callback_function;
}
