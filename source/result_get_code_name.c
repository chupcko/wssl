#include "main.h"

static struct
{
  wssl_result_code_e code;
  char*              name;
} Wssl_result_code_table[] =
{
  #define CALL(what_id, what_number, what_name) { WSSL_RESULT_CODE_##what_id, what_name },
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  { WSSL_RESULT_CODE_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_result_get_code_name
(
  _WSSL_IN_ const wssl_result_t result
)
{
  wssl_size_t i;
  for(i = 0; Wssl_result_code_table[i].name != WSSL_NULL; i++)
    if(Wssl_result_code_table[i].code == result.code)
      return Wssl_result_code_table[i].name;
  return "Unknown";
}
