#include "main.h"

#define BASE64_ENCODE_CHECK(what_size)                         \
do                                                             \
  if(*output_length+(what_size) > output_size)                 \
    return MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "base64"); \
while(false)                                                   \

#define BASE64_ENCODE_PUT(what_data)    \
do                                      \
{                                       \
  output[*output_length] = (what_data); \
  (*output_length)++;                   \
}                                       \
while(false)                            \

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
    BASE64_ENCODE_CHECK(4);
    BASE64_ENCODE_PUT(table[(input[i]>>2)&0x3f]);
    BASE64_ENCODE_PUT(table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
    BASE64_ENCODE_PUT(table[((input[i+1]&0xf)<<2) | ((input[i+2]>>6)&0x3)]);
    BASE64_ENCODE_PUT(table[input[i+2]&0x3f]);
  }
  switch(input_size-i)
  {
    case 2:
      BASE64_ENCODE_CHECK(4);
      BASE64_ENCODE_PUT(table[(input[i]>>2)&0x3f]);
      BASE64_ENCODE_PUT(table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
      BASE64_ENCODE_PUT(table[(input[i+1]&0xf)<<2]);
      BASE64_ENCODE_PUT('=');
      break;
    case 1:
      BASE64_ENCODE_CHECK(4);
      BASE64_ENCODE_PUT(table[(input[i]>>2)&0x3f]);
      BASE64_ENCODE_PUT(table[(input[i]&0x3)<<4]);
      BASE64_ENCODE_PUT('=');
      BASE64_ENCODE_PUT('=');
      break;
  }

  BASE64_ENCODE_CHECK(1);
  output[*output_length] = '\0';

  PASS;
}
