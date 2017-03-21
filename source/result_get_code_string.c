#include "main.h"

static struct
{
  wssl_result_code_e code;
  char*              string;
} Wssl_result_code_strings_table[] =
{
  #define CALL(what_name, what_number, what_string) { WSSL_RESULT_CODE_##what_name, what_string },
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  { WSSL_RESULT_CODE_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_result_get_code_string
(
  _WSSL_IN_ const wssl_result_t result
)
{
  wssl_size_t i;
  for(i = 0; Wssl_result_code_strings_table[i].string != WSSL_NULL; i++)
    if(Wssl_result_code_strings_table[i].code == result.code)
      return Wssl_result_code_strings_table[i].string;
  return "Unknown";
}
