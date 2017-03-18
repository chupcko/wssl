#include "main.h"

typedef uint32_t sha1_word_t;
typedef uint64_t sha1_length_t;

#define SHA1_WORD_SIZE_IN_OCTETS   sizeof(sha1_word_t)
#define SHA1_WORD_SIZE_IN_BITS     (SHA1_WORD_SIZE_IN_OCTETS*WSSL_OCTET_SIZE_IN_BITS)
#define SHA1_LENGTH_SIZE_IN_OCTETS sizeof(sha1_length_t)
#define SHA1_CHUNK_SIZE_IN_BITS    512
#define SHA1_CHUNK_SIZE_IN_OCTETS  (SHA1_CHUNK_SIZE_IN_BITS/WSSL_OCTET_SIZE_IN_BITS)
#define SHA1_CHUNK_SIZE_IN_WORDS   (SHA1_CHUNK_SIZE_IN_OCTETS/SHA1_WORD_SIZE_IN_OCTETS)
#define SHA1_RESULT_SIZE_IN_WORDS  (SHA1_RESULT_SIZE_IN_BITS/SHA1_WORD_SIZE_IN_BITS)
#define SHA1_ROUND_NUMBER          (SHA1_CHUNK_SIZE_IN_WORDS*SHA1_RESULT_SIZE_IN_WORDS)
#define SHA1_ROUND_NUMBER_QUARTER  (SHA1_ROUND_NUMBER/4)

typedef wssl_octet_t sha1_chunk_t[SHA1_CHUNK_SIZE_IN_OCTETS];
typedef sha1_word_t sha1_intermediate_result_t[SHA1_RESULT_SIZE_IN_WORDS];

static inline sha1_word_t sha1_word_rotate_left
(
  _WSSL_IN_ const sha1_word_t word,
  _WSSL_IN_ const wssl_size_t i
)
{
  return (word<<i)|(word>>(SHA1_WORD_SIZE_IN_BITS-i));
}

static void sha1_processing_chunk
(
  _WSSL_IN_  const sha1_chunk_t               chunk,
  _WSSL_OUT_       sha1_intermediate_result_t intermediate_result
)
{
  wssl_size_t i;
  wssl_size_t j;
  sha1_intermediate_result_t r;
  sha1_word_t w[SHA1_ROUND_NUMBER];
  sha1_word_t t;

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

_FUNCTION_
void wssl_sha1
(
  _WSSL_IN_  const wssl_octet_t*      data,
  _WSSL_IN_  const wssl_size_t        data_size,
  _WSSL_OUT_       wssl_sha1_result_t result
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
  wssl_size_t i;
  wssl_ssize_t j;

  wssl_ssize_t size;
  for(size = 0; size+SHA1_CHUNK_SIZE_IN_OCTETS <= data_size; size += SHA1_CHUNK_SIZE_IN_OCTETS)
    sha1_processing_chunk(&data[size], intermediate_result);

  sha1_chunk_t chunk;
  bzero((void*)chunk, SHA1_CHUNK_SIZE_IN_OCTETS);
  if(data_size-size > 0)
    memcpy((void*)chunk, (void*)&data[size], (size_t)(data_size-size));
  chunk[data_size-size] = 0x80;

  if(data_size-size >= SHA1_CHUNK_SIZE_IN_OCTETS-SHA1_LENGTH_SIZE_IN_OCTETS)
  {
    sha1_processing_chunk(chunk, intermediate_result);
    bzero((void*)chunk, SHA1_CHUNK_SIZE_IN_OCTETS);
  }

  sha1_length_t length = (sha1_length_t)data_size*WSSL_OCTET_SIZE_IN_BITS;
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
