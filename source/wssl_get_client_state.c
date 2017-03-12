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
  int i;

  for(i = 0; Wssl_client_state_table[i].string != NULL; i++)
    if(Wssl_client_state_table[i].state == state)
      return Wssl_client_state_table[i].string;
  return "Unknown";
}
