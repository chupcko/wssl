#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send_frame
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_frame_t*  frame
)
{
  wssl_chunk_t* chunk;
  WSSL_CALL(wssl_chunk_add(client, wssl_frame_buffer_size(frame), &chunk));

  WSSL_CALL(wssl_frame_put(frame, chunk->buffer.data, chunk->buffer.size, &chunk->buffer.end));
  WSSL_CALL(wssl_client_do_send(client));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
