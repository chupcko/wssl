#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <inttypes.h>
#include <time.h>

typedef uint8_t wssl_octet_t;
typedef int wssl_size_t;

#define WSSL_OCTET_SIZE_IN_BITS 8

#define WSSL_SHA1_RESULT_SIZE_IN_BITS   160
#define WSSL_SHA1_RESULT_SIZE_IN_OCTETS (WSSL_SHA1_RESULT_SIZE_IN_BITS/WSSL_OCTET_SIZE_IN_BITS)
typedef wssl_octet_t wssl_sha1_result_t[WSSL_SHA1_RESULT_SIZE_IN_OCTETS];

typedef uint32_t sha1_word_t;
typedef uint64_t sha1_length_t;

#define SHA1_WORD_SIZE_IN_OCTETS   sizeof(sha1_word_t)
#define SHA1_WORD_SIZE_IN_BITS     (SHA1_WORD_SIZE_IN_OCTETS*WSSL_OCTET_SIZE_IN_BITS)
#define SHA1_LENGTH_SIZE_IN_OCTETS sizeof(sha1_length_t)
#define SHA1_CHUNK_SIZE_IN_BITS    512
#define SHA1_CHUNK_SIZE_IN_OCTETS  (SHA1_CHUNK_SIZE_IN_BITS/WSSL_OCTET_SIZE_IN_BITS)
#define SHA1_CHUNK_SIZE_IN_WORDS   (SHA1_CHUNK_SIZE_IN_OCTETS/SHA1_WORD_SIZE_IN_OCTETS)
#define SHA1_RESULT_SIZE_IN_WORDS  (WSSL_SHA1_RESULT_SIZE_IN_BITS/SHA1_WORD_SIZE_IN_BITS)
#define SHA1_ROUND_NUMBER          (SHA1_CHUNK_SIZE_IN_WORDS*SHA1_RESULT_SIZE_IN_WORDS)
#define SHA1_ROUND_NUMBER_QUARTER  (SHA1_ROUND_NUMBER/4)

typedef wssl_octet_t sha1_chunk_t[SHA1_CHUNK_SIZE_IN_OCTETS];
typedef sha1_word_t sha1_intermediate_result_t[SHA1_RESULT_SIZE_IN_WORDS];

static inline sha1_word_t sha1_word_rotate_left
(
  const sha1_word_t word,
  const wssl_size_t i
)
{
  return (word<<i)|(word>>(SHA1_WORD_SIZE_IN_BITS-i));
}

static void sha1_processing_chunk
(
  const sha1_chunk_t chunk,
  sha1_intermediate_result_t intermediate_result
)
{
  sha1_intermediate_result_t r;
  sha1_word_t w[SHA1_ROUND_NUMBER];
  sha1_word_t t;
  wssl_size_t i;
  wssl_size_t j;

  for(i = 0; i < SHA1_RESULT_SIZE_IN_WORDS; i++)
    r[i] = intermediate_result[i];

  for(i = 0; i < SHA1_ROUND_NUMBER; i++)
  {
    if(i < SHA1_CHUNK_SIZE_IN_WORDS)
    {
      w[i] = 0;
      for(j = 0; j < SHA1_WORD_SIZE_IN_OCTETS; j++)
      {
        w[i] <<= WSSL_OCTET_SIZE_IN_BITS;
        w[i] |= (sha1_word_t)chunk[i*SHA1_WORD_SIZE_IN_OCTETS+j];
      }
    }
    else
      w[i] = sha1_word_rotate_left(w[i-3]^w[i-8]^w[i-14]^w[i-16], 1);

    t = sha1_word_rotate_left(r[0], 5)+r[4]+w[i];
    switch(i/SHA1_ROUND_NUMBER_QUARTER)
    {
      case 0:
        t += (r[3]^(r[1]&(r[2]^r[3])))+0x5a827999;
        break;
      case 1:
        t += (r[1]^r[2]^r[3])+0x6ed9eba1;
        break;
      case 2:
        t += ((r[1]&r[2])|(r[3]&(r[1]|r[2])))+0x8f1bbcdc;
        break;
      case 3:
        t += (r[1]^r[2]^r[3])+0xca62c1d6;
        break;
    }
    r[4] = r[3];
    r[3] = r[2];
    r[2] = sha1_word_rotate_left(r[1], 30);
    r[1] = r[0];
    r[0] = t;
  }

  for(i = 0; i < SHA1_RESULT_SIZE_IN_WORDS; i++)
    intermediate_result[i] += r[i];
}

void wssl_sha1
(
  const wssl_octet_t* data,
  const wssl_size_t data_size,
  wssl_sha1_result_t result
)
{
  sha1_intermediate_result_t intermediate_result =
  {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476,
    0xc3d2e1f0
  };
  wssl_size_t size;
  sha1_chunk_t chunk;
  sha1_length_t length;
  wssl_size_t i;
  wssl_size_t j;

  for(size = 0; size <= data_size-SHA1_CHUNK_SIZE_IN_OCTETS; size += SHA1_CHUNK_SIZE_IN_OCTETS)
    sha1_processing_chunk(&data[size], intermediate_result);

  bzero((void*)chunk, SHA1_CHUNK_SIZE_IN_OCTETS);
  if(data_size-size > 0)
    memcpy((void*)chunk, (void*)&data[size], (size_t)(data_size-size));
  chunk[data_size-size] = 0x80;

  if(data_size-size >= SHA1_CHUNK_SIZE_IN_OCTETS-SHA1_LENGTH_SIZE_IN_OCTETS)
  {
    sha1_processing_chunk(chunk, intermediate_result);
    bzero((void*)chunk, SHA1_CHUNK_SIZE_IN_OCTETS);
  }

  length = (sha1_length_t)data_size*WSSL_OCTET_SIZE_IN_BITS;
  for(i = 0; i < SHA1_LENGTH_SIZE_IN_OCTETS; i++)
  {
    chunk[SHA1_CHUNK_SIZE_IN_OCTETS-1-i] = (wssl_octet_t)(length&0xff);
    length >>= WSSL_OCTET_SIZE_IN_BITS;
  }
  sha1_processing_chunk(chunk, intermediate_result);

  for(i = 0; i < SHA1_RESULT_SIZE_IN_WORDS; i++)
    for(j = SHA1_WORD_SIZE_IN_OCTETS-1; j >= 0; j--)
    {
      result[i*SHA1_WORD_SIZE_IN_OCTETS+j] = (wssl_octet_t)(intermediate_result[i]&0xff);
      intermediate_result[i] >>= WSSL_OCTET_SIZE_IN_BITS;
    }
}





void d(wssl_sha1_result_t result)
{
  int i;

  for(i = 0; i < WSSL_SHA1_RESULT_SIZE_IN_OCTETS; i++)
    printf("%02" PRIx8, result[i]);
  printf("\n");
}

#define N 100000
#define SIZE 10000

#include <openssl/sha.h>

void test_single(wssl_octet_t* data, wssl_size_t data_size)
{
  wssl_sha1_result_t result_wssl;
  wssl_sha1_result_t result_openssl;
  int i;

  wssl_sha1(data, data_size, result_wssl);
  SHA1(data, data_size, result_openssl);

  for(i = 0; i < WSSL_SHA1_RESULT_SIZE_IN_OCTETS; i++)
    if(result_wssl[i] != result_openssl[i])
    {
      d(result_wssl);
      d(result_openssl);
    }
}

void test(void)
{
  int i;
  wssl_octet_t data[SIZE];
  wssl_size_t data_size;
  int j;

  for(i = 0; i < N; i++)
  {
    data_size = rand()%SIZE;
    for(j = 0; j < data_size; j++)
      data[j] = rand()&0xff;
    test_single(data, data_size);
  }
}

int main(void)
{
  srand(time(NULL));

  test();
  printf("DONE\n");

  wssl_sha1_result_t result;

  #define MESSAGE ""
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefgh"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefg"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgh"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgha"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefg"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgh"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE

  #define MESSAGE "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgha"
  wssl_sha1(MESSAGE, sizeof MESSAGE-1, result);
  d(result);
  #undef MESSAGE
  return EXIT_SUCCESS;
}

/*
da39a3ee5e6b4b0d3255bfef95601890afd80709
425af12a0743502b322e93a015bcf868e324d56a
6b70fc1b4da1922a70686a0c4dcef6512ff5c65c
dc81d69ad00bff0c312e703e40a19ab8e732416e
b3a5ac39b037fc395fad28de3cd7b2bd3e564d11
8495e26e0b2ad0cf2aebff079beb982648954212
cbd8c1ee0779118216562290579a902da9c7080a
4021e411111ddeb75c3a7c6e93db91b5c2a93aa3
*/
