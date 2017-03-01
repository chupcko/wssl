#include "main.h"

int handshake_calculate(char* input, char* output, int output_size)
{
  #define HANDSHAKE_CALCULATE_MAGIC "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

  int data_size = strlen(input)+sizeof HANDSHAKE_CALCULATE_MAGIC;
  char data[data_size];
  int data_length;
  unsigned char hash[SHA_DIGEST_LENGTH];

  data_length = snprintf(data, data_size, "%s" HANDSHAKE_CALCULATE_MAGIC, input);
  SHA1(data, data_length, hash);
  return base64_encode((uint8_t*)hash, SHA_DIGEST_LENGTH, output, output_size);

  #undef HANDSHAKE_CALCULATE_MAGIC
}
