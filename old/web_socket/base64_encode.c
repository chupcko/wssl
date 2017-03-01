#include "main.h"

int base64_encode(uint8_t* input, int input_size, char* output, int output_size)
{
  #define BASE64_ENCODE_TABLE_SIZE 64
  #define BASE64_ENCODE_PUT(output, output_length, output_size, what) \
  do                                                                  \
  {                                                                   \
    if(output_length >= output_size)                                  \
      return -1;                                                      \
    output[output_length] = what;                                     \
    output_length++;                                                  \
  }                                                                   \
  while(0)                                                            \

  static char table[BASE64_ENCODE_TABLE_SIZE] =
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
    BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
    BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
    BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i+1]&0xf)<<2) | ((input[i+2]>>6)&0x3)]);
    BASE64_ENCODE_PUT(output, output_length, output_size, table[input[i+2]&0x3f]);
  }
  switch(input_size-i)
  {
    case 2:
      BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
      BASE64_ENCODE_PUT(output, output_length, output_size, table[((input[i]&0x3)<<4) | ((input[i+1]>>4)&0xf)]);
      BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i+1]&0xf)<<2]);
      BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      break;
    case 1:
      BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]>>2)&0x3f]);
      BASE64_ENCODE_PUT(output, output_length, output_size, table[(input[i]&0x3)<<4]);
      BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      BASE64_ENCODE_PUT(output, output_length, output_size, '=');
      break;
  }
  BASE64_ENCODE_PUT(output, output_length, output_size, '\0');
  return output_length-1;

  #undef BASE64_ENCODE_PUT
  #undef BASE64_ENCODE_TABLE_SIZE
}
