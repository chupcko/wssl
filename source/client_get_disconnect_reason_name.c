#include "main.h"

static struct
{
  wssl_client_disconnect_reason_e disconnect_reason;
  char*                           name;
} Wssl_client_disconnect_reason_table[] =
{
  #define CALL(what_id, what_name) { WSSL_CLIENT_DISCONNECT_REASON_##what_id, what_name },
  WSSL_CLIENT_DISCONNECT_REASON_TABLE(CALL)
  #undef CALL
  { WSSL_CLIENT_DISCONNECT_REASON_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_client_get_disconnect_reason_name
(
  _WSSL_IN_ const wssl_client_disconnect_reason_e disconnect_reason
)
{
  wssl_size_t i;
  for(i = 0; Wssl_client_disconnect_reason_table[i].name != WSSL_NULL; i++)
    if(Wssl_client_disconnect_reason_table[i].disconnect_reason == disconnect_reason)
      return Wssl_client_disconnect_reason_table[i].name;
  return "Unknown";
}
