#include "main.h"

static struct
{
  wssl_result_code_t code;
  char*              string;
} Wssl_result_strings_table[] =
{
  #define CALL(name, number, string) { WSSL_RESULT_CODE_##name, string },
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  { WSSL_RESULT_CODE_END_, NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_get_result_string
(
  _WSSL_IN_ const wssl_result_t result
)
{
  int i;

  for(i = 0; Wssl_result_strings_table[i].string != NULL; i++)
    if(Wssl_result_strings_table[i].code == result.code)
      return Wssl_result_strings_table[i].string;
  return "Unknown";
}
