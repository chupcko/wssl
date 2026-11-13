#include "main.h"

_FUNCTION_
wssl_result_t wssl_handshake
(
  _WSSL_IN_  const wssl_string_t* key,
  _WSSL_OUT_       char*          output,
  _WSSL_IN_  const wssl_size_t    output_size,
  _WSSL_OUT_       wssl_size_t*   output_length
)
{
  *output_length = 0;

  wssl_size_t data_size = key->data_length+sizeof(HANDSHAKE_MAGIC);
  char* data = (char*)malloc((size_t)data_size);
  if(data == NULL)
  {
    FAIL_ERROR("handshake", WSSL_RESULT_CODE_ERROR_NO_MEMORY);
  }

  wssl_ssize_t data_length = snprintf(data, (size_t)data_size, "%s" HANDSHAKE_MAGIC, key->data);
  WSSL_ASSERT(data_length == data_size-1);

  WSSL_ASSERT(SHA1_RESULT_SIZE_IN_OCTETS == SHA_DIGEST_LENGTH);

  wssl_sha1_result_t sha1_result;
  if(SHA1((wssl_octet_t*)data, (size_t)data_length, sha1_result) == NULL)
  {
    free((void*)data);
    FAIL_ERROR("SHA1", WSSL_RESULT_CODE_ERROR_SSL);
  }
  free((void*)data);

  TRY_CALL(wssl_base64_encode(sha1_result, SHA1_RESULT_SIZE_IN_OCTETS, output, output_size, output_length));

  PASS;
}
