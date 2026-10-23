#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_add_ssl
(
  _WSSL_MODIFY_ wssl_t*      wssl,
  _WSSL_IN_     const char*  ip,
  _WSSL_IN_     const int    port,
  _WSSL_IN_     const bool   ipv6,
  _WSSL_IN_     const char*  certificate_file,
  _WSSL_IN_     const char*  private_key_file
)
{
  TRY_CALL(wssl_server_add(wssl, ip, port, WSSL_SSL, ipv6));

  wssl_server_t* server = wssl_server_chain_get_entry_from_wssl_chain_link(wssl->servers.prev);
  wssl_result_t result = wssl_server_enable_ssl(server, certificate_file, private_key_file);
  if(wssl_result_is_not_ok(result))
  {
    wssl_result_t delete_result = wssl_server_delete(server);
    if(wssl_result_is_not_ok(delete_result))
      return delete_result;
    return result;
  }

  PASS;
}
