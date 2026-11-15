#include "main.h"

_LIBRARY_FUNCTION_
void wssl_logging_end
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  if(wssl->logging.descriptor != WSSL_NO_DESCRIPTOR)
  {
    close(wssl->logging.descriptor);
    wssl->logging.descriptor = WSSL_NO_DESCRIPTOR;
  }
}
