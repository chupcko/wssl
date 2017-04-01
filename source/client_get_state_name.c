#include "main.h"

static struct
{
  wssl_client_state_e state;
  char*               name;
} Wssl_client_state_table[] =
{
  #define CALL(what_id, what_name) { WSSL_CLIENT_STATE_##what_id, what_name },
  WSSL_CLIENT_STATE_TABLE(CALL)
  #undef CALL
  { WSSL_CLIENT_STATE_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_client_get_state_name
(
  _WSSL_IN_ const wssl_client_state_e state
)
{
  wssl_size_t i;
  for(i = 0; Wssl_client_state_table[i].name != WSSL_NULL; i++)
    if(Wssl_client_state_table[i].state == state)
      return Wssl_client_state_table[i].name;
  return "Unknown";
}
