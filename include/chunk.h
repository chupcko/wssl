#ifndef _CHUNK_H_
#define _CHUNK_H_

_INCLUDE_BEGIN_

typedef struct wssl_chunk_t
{
  wssl_chunk_chain_t chain_link;
  wssl_buffer_t      buffer;
  wssl_octet_t       data[];
} wssl_chunk_t;

_INCLUDE_END_

static inline
void wssl_chunk_delete
(
  _WSSL_MODIFY_ wssl_chunk_t* chunk
)
{
  wssl_chunk_chain_delete_link(&chunk->chain_link);
  free((void*)chunk);
}

#endif
