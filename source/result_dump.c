#include "main.h"

_LIBRARY_FUNCTION_
void wssl_result_dump
(
  _WSSL_IN_ const wssl_result_t result,
  _WSSL_IN_       FILE*         file
)
{
  fprintf(file, "\"%s\"", wssl_result_get_code_string(result));
  switch(result.code)
  {
    case WSSL_RESULT_CODE_OK:
      break;
    case WSSL_RESULT_CODE_ERROR_ERRNO:
      fprintf
      (
        file,
        " on \"%s\" with %d:\"%s\"",
        result.where,
        result.system_errno,
        strerror(result.system_errno)
      );
      break;
    default:
      fprintf(file, " on \"%s\"", result.where);
      break;
  }
  fprintf(file, "\n");
}
