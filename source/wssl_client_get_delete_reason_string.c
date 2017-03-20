#include "main.h"

static struct
{
  wssl_client_delete_reason_e delete_reason;
  char*                       string;
} Wssl_client_delete_reason_table[] =
{
  #define CALL(what_name, what_string) { WSSL_CLIENT_DELETE_REASON_##what_name, what_string },
  WSSL_CLIENT_DELETE_REASON_TABLE(CALL)
  #undef CALL
  { WSSL_CLIENT_DELETE_REASON_END_, WSSL_NULL }
};

_LIBRARY_FUNCTION_
const char* wssl_client_get_delete_reason_string
(
  _WSSL_IN_ const wssl_client_delete_reason_e delete_reason
)
{
  wssl_size_t table_index;
  for(table_index = 0; Wssl_client_delete_reason_table[table_index].string != WSSL_NULL; table_index++)
    if(Wssl_client_delete_reason_table[table_index].delete_reason == delete_reason)
      return Wssl_client_delete_reason_table[table_index].string;
  return "Unknown";
}
