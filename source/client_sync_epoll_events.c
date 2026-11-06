#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_sync_epoll_events
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  uint32_t events;
  if(client->ssl != NULL && client->ssl_handshake_done == false)
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

  if(client->epoll_event.events != events)
  {
    client->epoll_event.events = events;
    if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
      FAIL_ERRNO("epoll_ctl", errno);
  }

  PASS;
}
