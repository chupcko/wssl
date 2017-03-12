#include "main.h"

_LIBRARY_FUNCTION_
void wssl_id_dump
(
  _WSSL_IN_ const wssl_id_t*  id,
  _WSSL_IN_       FILE*       file
)
{
  fprintf(file, "%" PRIu16 ":%" PRIu32, id->prefix, id->suffix);
}
