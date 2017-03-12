#include "main.h"

_FUNCTION_
int wssl_base64_encode
(
  _WSSL_IN_  const wssl_octet_t* input,
  _WSSL_IN_  const int           input_size,
  _WSSL_OUT_       char*         output,
  _WSSL_IN_  const wssl_size_t   output_size
)
{
  #define WSSL_BASE64_ENCODE_PUT(what_output, what_output_length, what_output_size, what_data) \
  do                                                                                           \
  {                                                                                            \
    if(what_output_length >= what_output_size)                                                 \
      return -1;                                                                               \
    what_output[what_output_length] = what_data;                                               \
    what_output_length++;                                                                      \
  }                                                                                            \
  while(0)                                                                                     \

  static char table[] =
  {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  };

  int output_length = 0;
  int i;

  for(i = 0; input_size-i >= 3; i += 3)
  {
    WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
    WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
    WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i+1]&0xf)<<2) | ((input[i+2]>>6)&0x3)]);
    WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[input[i+2]&0x3f]);
  }
  switch(input_size-i)
  {
    case 2:
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i+1]&0xf)<<2]);
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      break;
    case 1:
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]&0x3)<<4]);
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      break;
  }
  WSSL_BASE64_ENCODE_PUT(output, output_length, output_size, '\0');
  return output_length-1;

  #undef WSSL_BASE64_ENCODE_PUT
}
