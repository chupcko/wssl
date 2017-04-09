#include "main.h"

static inline
wssl_result_t wssl_client_do_send_set_epoll_event
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  uint32_t events = client->epoll_event.events;
  if(wssl_chunk_chain_is_empty(&client->output_chunks))
    events &= ~EPOLLOUT;
  else
    events |= EPOLLOUT;

  if(client->epoll_event.events != events)
  {
    client->epoll_event.events = events;
    if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
      return MAKE_RESULT_ERRNO("epoll_ctl", errno);
  }

  PASS;
}

_FUNCTION_
wssl_result_t wssl_client_do_send
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  wssl_ssize_t send_size;
  wssl_chunk_chain_t* chunk_link;
  wssl_chunk_chain_t* chunk_link_next;
  wssl_chunk_t* chunk;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(chunk_link, chunk_link_next, &client->output_chunks)
  {
    chunk = wssl_chunk_chain_get_entry_from_chain_link(chunk_link);
    while(wssl_buffer_is_not_empty(&chunk->buffer))
    {
      send_size = (wssl_ssize_t)send
      (
        client->socket_descriptor,
        (void*)&chunk->buffer.data[chunk->buffer.begin],
        (size_t)(chunk->buffer.end-chunk->buffer.begin),
        0
      );
      if(send_size < 0)
        switch(errno)
        {
          case EAGAIN:
            PASS;
            break;
          case ECONNRESET:
          case EPIPE:
            MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
            break;
          default:
            return MAKE_RESULT_ERRNO("send", errno);
            break;
        }
      if(send_size == 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);

      chunk->buffer.begin += (wssl_size_t)send_size;
    }
    wssl_chunk_delete(chunk);
    client->number_of_output_chunks--;
  }

  TRY_CALL(wssl_client_do_send_set_epoll_event(client));

  PASS;
}
