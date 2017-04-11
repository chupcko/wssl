#include "main.h"

_FUNCTION_
void wssl_client_purge
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  close(client->socket_descriptor);
  client->socket_descriptor = WSSL_NO_DESCRIPTOR;

  if(wssl_buffer_is_allocated(&client->input_buffer))
    wssl_buffer_free(&client->input_buffer);

  wssl_chunk_chain_t* chunk_link;
  wssl_chunk_chain_t* chunk_link_next;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(chunk_link, chunk_link_next, &client->output_chunks)
    wssl_chunk_delete(wssl_chunk_chain_get_entry_from_client_chain_link(chunk_link));

  wssl_header_clean(&client->header);
  if(wssl_frame_is_allocated(&client->frame))
    wssl_frame_free(&client->frame);

  wssl_client_chain_delete_link(&client->server_chain_link);
  wssl_client_chain_delete_link(&client->wssl_chain_link);

  free((void*)client);
}
