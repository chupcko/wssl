#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  wssl_ssize_t recv_size;
  wssl_size_t processed;

  if(WSSL_BUFFER_IS_NOT_SET(client->input_buffer))
    WSSL_BUFFER_CREATE(client->input_buffer, WSSL_BUFFER_SIZE, "input_buffer");

  recv_size = (wssl_ssize_t)recv
  (
    client->socket_descriptor,
    (void*)&client->input_buffer.data[client->input_buffer.used],
    (size_t)(client->input_buffer.size-1-client->input_buffer.used),
    0
  );
  if(recv_size < 0)
  {
    if(errno == ECONNRESET || errno == EPIPE)
    {
      WSSL_CALL(wssl_client_delete(wssl, client));
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
    }
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "recv", errno);
  }
  if(recv_size == 0)
  {
    WSSL_CALL(wssl_client_delete(wssl, client));
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
  }
  client->input_buffer.used += recv_size;
  client->input_buffer.data[client->input_buffer.used] = '\0';

  /*# obradi buffer */
printf("\"%s\"\n", client->input_buffer.data);
processed = 2;

  if(processed == recv_size)
    WSSL_BUFFER_DELETE(client->input_buffer);
  else if(processed > 0)
    WSSL_BUFFER_MOVE(client->input_buffer, processed);
  else if(client->input_buffer.used == client->input_buffer.size-1)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "input_buffer", 0);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
