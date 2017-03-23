#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_delete
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(client->wssl->disconnect_callback != WSSL_CALLBACK_NONE)
    (*client->wssl->disconnect_callback)(client, client->disconnect_reason);

  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_DEL, client->socket_descriptor, NULL) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);

  if(close(client->socket_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "close", errno);
  client->socket_descriptor = WSSL_NO_DESCRIPTOR;

  if(wssl_buffer_is_allocated(&client->input_buffer))
    wssl_buffer_free(&client->input_buffer);

  wssl_chunk_chain_t* chunk_link;
  wssl_chunk_chain_t* chunk_link_next;
  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(chunk_link, chunk_link_next, &client->output_chunks)
    wssl_chunk_delete(wssl_chunk_chain_entry(chunk_link));

  wssl_header_clean(&client->header);
  if(wssl_frame_is_allocated(&client->frame))
    wssl_frame_free(&client->frame);

  wssl_client_chain_delete_link(&client->chain_link);

  free((void*)client);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
