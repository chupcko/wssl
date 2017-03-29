#include "main.h"

static inline
wssl_result_t wssl_base64_encode_put
(
  _WSSL_MODIFY_       char*        output,
  _WSSL_MODIFY_       wssl_size_t* output_index,
  _WSSL_IN_     const wssl_size_t  output_size,
  _WSSL_IN_     const char         data
)
{
  if(*output_index >= output_size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "base64");
  output[*output_index] = data;
  (*output_index)++;

  return WSSL_MAKE_RESULT_OK;
}

_FUNCTION_
wssl_result_t wssl_base64_encode
(
  _WSSL_IN_  const wssl_octet_t* input,
  _WSSL_IN_  const wssl_size_t   input_size,
  _WSSL_OUT_       char*         output,
  _WSSL_IN_  const wssl_size_t   output_size,
  _WSSL_OUT_       wssl_size_t*  output_length
)
{
  static char table[] =
  {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  };
  wssl_size_t i;

  *output_length = 0;

  for(i = 0; input_size-i >= 3; i += 3)
  {
    WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[(input[i]>>2)&0x3f]));
    WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]));
    WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[((input[i+1]&0xf)<<2) | ((input[i+2]>>6)&0x3)]));
    WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[input[i+2]&0x3f]));
  }
  switch(input_size-i)
  {
    case 2:
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[(input[i]>>2)&0x3f]));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[(input[i+1]&0xf)<<2]));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, '='));
      break;
    case 1:
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[(input[i]>>2)&0x3f]));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, table[(input[i]&0x3)<<4]));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, '='));
      WSSL_CALL(wssl_base64_encode_put(output, output_length, output_size, '='));
      break;
  }

  if(*output_length >= output_size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "base64");
  output[*output_length] = '\0';

  return WSSL_MAKE_RESULT_OK;
}
