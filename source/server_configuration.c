#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_ip
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  ip
)
{
  wssl_str_copy(configuration->ip, sizeof(configuration->ip), ip);

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_port
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     int                          port
)
{
  configuration->port = port;

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_ipv6
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     bool                         ipv6
)
{
  configuration->ipv6 = ipv6;

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_ipv6_dual_stack
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     bool                         ipv6_dual_stack
)
{
  configuration->ipv6_dual_stack = ipv6_dual_stack;

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_ssl
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     bool                         ssl
)
{
  configuration->ssl = ssl;

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_enable_ssl
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  certificate_file,
  _WSSL_IN_     const char*                  private_key_file
)
{
  TRY_CALL(wssl_server_configuration_set_certificate_files(configuration, certificate_file, private_key_file));

  return wssl_server_configuration_set_ssl(configuration, true);
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_configuration_set_certificate_files
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  certificate_file,
  _WSSL_IN_     const char*                  private_key_file
)
{
  wssl_str_copy(configuration->certificate_file, sizeof(configuration->certificate_file), certificate_file);
  wssl_str_copy(configuration->private_key_file, sizeof(configuration->private_key_file), private_key_file);

  PASS;
}
