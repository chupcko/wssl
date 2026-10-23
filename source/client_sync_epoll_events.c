#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_sync_epoll_events
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  uint32_t events;
  if(client->ssl != WSSL_NULL && !client->ssl_handshake_done)
    if(client->ssl_want == SSL_ERROR_WANT_WRITE)
      events = EPOLLOUT;
    else
      events = EPOLLIN;
  else
  {
    events = EPOLLIN;
    if(wssl_chunk_chain_is_not_empty(&client->output_chunks))
      events |= EPOLLOUT;
    if(client->ssl_want == SSL_ERROR_WANT_WRITE)
      events |= EPOLLOUT;
  }

  TRY_CALL(wssl_client_set_epoll_events(client, events));

  PASS;
}
