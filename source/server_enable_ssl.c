#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_server_enable_ssl
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  SSL_CTX* ssl_ctx = SSL_CTX_new(TLS_server_method());
  if(ssl_ctx == NULL)
  {
    FAIL_ERROR("SSL_CTX_new", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_use_certificate_chain_file(ssl_ctx, server->configuration.certificate_file) != 1)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_use_certificate_chain_file", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_use_PrivateKey_file(ssl_ctx, server->configuration.private_key_file, SSL_FILETYPE_PEM) != 1)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_use_PrivateKey_file", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_check_private_key(ssl_ctx) != 1)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_check_private_key", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_set_min_proto_version(ssl_ctx, WSSL_SSL_MIN_PROTOCOL_VERSION) == 0)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_set_min_proto_version", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_set_cipher_list(ssl_ctx, WSSL_SSL_CIPHER_LIST) == 0)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_set_cipher_list", WSSL_RESULT_CODE_ERROR_SSL);
  }

  if(SSL_CTX_set_ciphersuites(ssl_ctx, WSSL_SSL_CIPHER_SUITES) == 0)
  {
    SSL_CTX_free(ssl_ctx);
    FAIL_ERROR("SSL_CTX_set_ciphersuites", WSSL_RESULT_CODE_ERROR_SSL);
  }

  server->ssl_ctx = ssl_ctx;

  PASS;
}
