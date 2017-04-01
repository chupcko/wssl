#include "main.h"

_FUNCTION_
wssl_result_t wssl_handshake
(
  _WSSL_IN_  const char*        input,
  _WSSL_IN_  const wssl_size_t  input_size,
  _WSSL_OUT_       char*        output,
  _WSSL_IN_  const wssl_size_t  output_size,
  _WSSL_OUT_       wssl_size_t* output_length
)
{
  *output_length = 0;

  wssl_size_t data_size = input_size+sizeof HANDSHAKE_MAGIC;
  char* data = (char*)malloc((size_t)data_size);
  if(data == NULL)
    return MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "handshake");

  wssl_ssize_t data_length = snprintf(data, (size_t)data_size, "%s" HANDSHAKE_MAGIC, input);
  WSSL_ASSERT(data_length == data_size-1);

  wssl_sha1_result_t sha1_result;
  wssl_sha1((wssl_octet_t*)data, (wssl_size_t)data_length, sha1_result);
  free((void*)data);

  CALL(wssl_base64_encode(sha1_result, SHA1_RESULT_SIZE_IN_OCTETS, output, output_size, output_length));

  return MAKE_RESULT_OK;
}
