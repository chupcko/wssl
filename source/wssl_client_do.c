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
  wssl_ssize_t local_processed;

  if(wssl_buffer_is_not_created(&client->input_buffer))
    WSSL_CALL(wssl_buffer_create(&client->input_buffer, BUFFER_SIZE));

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

printf("# %d\n", client->input_buffer.used);
  processed = 0;
  while(processed < client->input_buffer.used)
  {
printf("## %d %d \"%s\"\n", processed, client->input_buffer.used-processed, &client->input_buffer.data[processed]);
    WSSL_CALL(wssl_client_processing(wssl, client, processed, client->input_buffer.used-processed, &local_processed));
    if(local_processed < 0)
    {
      WSSL_CALL(wssl_client_delete(wssl, client));
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
    }
    if(local_processed == 0)
      break;
    processed += (wssl_size_t)local_processed;
wssl_client_dump(client, stdout, 0);
  }
printf("### %d %d\n", processed, client->input_buffer.used-processed);

  if(processed == client->input_buffer.used)
    wssl_buffer_clean(&client->input_buffer);
  else if(processed > 0)
    wssl_buffer_shift(&client->input_buffer, processed);
  else if(client->input_buffer.used == client->input_buffer.size-1)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "input_buffer", 0);

wssl_client_dump(client, stdout, 0);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
