#include "main.h"

_FUNCTION_
wssl_result_t wssl_chunk_add
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_size_t    size,
  _WSSL_OUT_          wssl_chunk_t** chunk
)
{
  if(client->number_of_output_chunks > client->wssl->max_number_of_output_chunks)
    MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_TOO_MUCH_OUTPUT_CHUNKS);

  *chunk = (wssl_chunk_t*)malloc
  (
    sizeof(wssl_chunk_t)+
    (size_t)size
  );
  if(*chunk == NULL)
    FAIL_ERROR("chunk", WSSL_RESULT_CODE_ERROR_NO_MEMORY);

  (*chunk)->buffer.data = (*chunk)->buffer_data;
  (*chunk)->buffer.size = size;
  (*chunk)->buffer.begin = 0;
  (*chunk)->buffer.end = 0;

  wssl_chunk_chain_add_link_backward(&client->output_chunks, &(*chunk)->client_chain_link);

  client->number_of_output_chunks++;

  PASS;
}
