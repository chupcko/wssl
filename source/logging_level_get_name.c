#include "main.h"

static struct
{
  wssl_logging_level_e level;
  char*                name;
} Wssl_logging_level_table[] =
{
  #define MACRO_CALL(what_id, what_number, what_name) { WSSL_LOGGING_LEVEL_##what_id, what_name },
  LOGGING_LEVEL_TABLE(MACRO_CALL)
  #undef MACRO_CALL
  { LOGGING_LEVEL_TABLE_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_logging_level_get_name
(
  _WSSL_IN_ const wssl_logging_level_e level
)
{
  wssl_size_t i;
  for(i = 0; Wssl_logging_level_table[i].name != WSSL_NULL; i++)
  {
    if(Wssl_logging_level_table[i].level == level)
    {
      return Wssl_logging_level_table[i].name;
    }
  }
  return "Unknown";
}
