#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_send_frame
(
  _WSSL_MODIFY_  wssl_client_t* client,
  _WSSL_IN_      wssl_frame_t*  frame
)
{
  wssl_buffer_t buffer;
  WSSL_CALL(wssl_buffer_allocate(&buffer, client->wssl->buffer_size_in_octets));
  WSSL_CALL(wssl_frame_put(frame, buffer.data, buffer.size, &buffer.used));
  wssl_result_t result = wssl_client_send(client, buffer.data, buffer.used);
  wssl_buffer_free(&buffer);
  if(wssl_result_is_not_ok(result))
    return result;
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
