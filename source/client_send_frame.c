#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send_frame
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_frame_t*  frame
)
{
  wssl_chunk_t* chunk;
  CALL(wssl_chunk_add(client, wssl_frame_buffer_size(frame), &chunk));

  CALL(wssl_frame_put(frame, chunk->buffer.data, chunk->buffer.size, &chunk->buffer.end));
  CALL(wssl_client_do_send(client));

  return MAKE_RESULT_OK;
}
