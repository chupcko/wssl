#include "main.h"

static struct
{
  wssl_client_state_e state;
  char*               string;
} Wssl_client_state_table[] =
{
  #define CALL(what_name, what_string) { WSSL_CLIENT_STATE_##what_name, what_string },
  WSSL_CLIENT_STATE_TABLE(CALL)
  #undef CALL
  { WSSL_CLIENT_STATE_END_, NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_get_client_state
(
  _WSSL_IN_ const wssl_client_state_e state
)
{
  wssl_size_t table_index;
  for(table_index = 0; Wssl_client_state_table[table_index].string != NULL; table_index++)
    if(Wssl_client_state_table[table_index].state == state)
      return Wssl_client_state_table[table_index].string;
  return "Unknown";
}
