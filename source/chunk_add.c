#include "main.h"

_FUNCTION_
wssl_result_t wssl_chunk_add
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_size_t    size,
  _WSSL_OUT_          wssl_chunk_t** chunk
)
{
  *chunk = (wssl_chunk_t*)malloc(sizeof(wssl_chunk_t)+(size_t)size);
  if(*chunk == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "chunk");

  /*# proveri da name previse buffera */

  (*chunk)->buffer.data = (*chunk)->data;
  (*chunk)->buffer.size = size;
  (*chunk)->buffer.begin = 0;
  (*chunk)->buffer.end = 0;

  wssl_chunk_chain_add_link_backward(&client->output_chunks, &(*chunk)->chain_link);

  return WSSL_MAKE_RESULT_OK;
}
