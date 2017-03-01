#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char data[] = "Hello, world!";
  unsigned char hash[SHA_DIGEST_LENGTH];
  int i;

  SHA1(data, strlen(data), hash);

  for(i = 0; i < SHA_DIGEST_LENGTH; i++)
    printf("%02x", (int)hash[i]);
  putchar('\n');

  return EXIT_SUCCESS;
}
