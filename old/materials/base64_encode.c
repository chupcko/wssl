#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE64_ENCODE_RESULT_SIZE(x) (((x)+2)/3*4)

int base64_encode(uint8_t* input, int input_size, char* output, int output_size)
{
  #define BASE64_ENCODE_TABLE_SIZE 64
  #define BASE64_ENCODE_PUT(output, output_length, output_size, what) \
  do                                                                  \
  {                                                                   \
    if(output_length >= output_size)                                  \
      return -output_length;                                          \
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

void test_base64_encode(char* input, char* excepted)
{
  int input_length = strlen(input);
  #define OUTPUT_SIZE 128
  char output[OUTPUT_SIZE];
  int output_length = base64_encode((uint8_t*)input, input_length, output, OUTPUT_SIZE);

  if(input_length <= 0)
    printf("ERROR: no space for base64_encode(\"%s\")\n", input);

  if(strcmp(output, excepted) != 0)
    printf("ERROR: bad resul base64_encode(\"%s\") = \"%s\", must be \"%s\"\n", input, output, excepted);

  if(BASE64_ENCODE_RESULT_SIZE(input_length) != output_length)
    printf("ERROR: bad size for BASE64_ENCODE_RESULT_SIZE(strlen(\"%s\")) = %d, must be %d\n", input, output_length, BASE64_ENCODE_RESULT_SIZE(input_length));
}

int main(void)
{
  test_base64_encode("Man", "TWFu");
  test_base64_encode("Ma",  "TWE=");
  test_base64_encode("M",   "TQ==");

  test_base64_encode("any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4=");
  test_base64_encode("any carnal pleasure",  "YW55IGNhcm5hbCBwbGVhc3VyZQ==");
  test_base64_encode("any carnal pleasur",   "YW55IGNhcm5hbCBwbGVhc3Vy");
  test_base64_encode("any carnal pleasu",    "YW55IGNhcm5hbCBwbGVhc3U=");
  test_base64_encode("any carnal pleas",     "YW55IGNhcm5hbCBwbGVhcw==");

  test_base64_encode("pleasure.", "cGxlYXN1cmUu");
  test_base64_encode("pleasure.", "cGxlYXN1cmUu");
  test_base64_encode("leasure.",  "bGVhc3VyZS4=");
  test_base64_encode("easure.",   "ZWFzdXJlLg==");
  test_base64_encode("asure.",    "YXN1cmUu");
  test_base64_encode("sure.",     "c3VyZS4=");

  printf("DONE\n");

  return EXIT_SUCCESS;
}
