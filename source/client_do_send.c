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
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

_FUNCTION_
wssl_result_t wssl_client_do_send
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  if(wssl_client_is_for_disconnecting(client))
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);

  wssl_ssize_t send_size;
  wssl_chunk_chain_t* chunk_link;
  wssl_chunk_chain_t* chunk_link_next;
  wssl_chunk_t* chunk;
  WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(chunk_link, chunk_link_next, &client->output_chunks)
  {
    chunk = wssl_chunk_chain_entry(chunk_link);
    while(wssl_buffer_is_not_empty(&chunk->buffer))
    {
      wssl_ssize_t send_size = (wssl_ssize_t)send
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
            return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
            break;
          case ECONNRESET:
          case EPIPE:
            wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
            return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
            break;
          default:
            return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "send", errno);
            break;
        }
      if(send_size == 0)
      {
        wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
      }
      chunk->buffer.begin += (wssl_size_t)send_size;
    }
    wssl_chunk_delete(chunk);
  }

  WSSL_CALL(wssl_client_do_send_set_epoll_event(client));

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
