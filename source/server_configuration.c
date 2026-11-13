#include "main.h"

_LIBRARY_FUNCTION_
void wssl_server_configuration_set_ip
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  ip
)
{
  wssl_str_copy(configuration->ip, sizeof(configuration->ip), ip);
}

_LIBRARY_FUNCTION_
void wssl_server_configuration_set_certificate_files
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  certificate_file,
  _WSSL_IN_     const char*                  private_key_file
)
{
  wssl_str_copy(configuration->certificate_file, sizeof(configuration->certificate_file), certificate_file);
  wssl_str_copy(configuration->private_key_file, sizeof(configuration->private_key_file), private_key_file);
}

_LIBRARY_FUNCTION_
void wssl_server_configuration_enable_ssl
(
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,
  _WSSL_IN_     const char*                  certificate_file,
  _WSSL_IN_     const char*                  private_key_file
)
{
  wssl_server_configuration_set_certificate_files(configuration, certificate_file, private_key_file);
  configuration->ssl = true;
}

