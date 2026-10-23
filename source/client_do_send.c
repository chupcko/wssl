#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_send
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(client->ssl != WSSL_NULL && !client->ssl_handshake_done)
  {
    TRY_CALL(wssl_client_do_ssl_handshake(client));
    PASS;
  }

  wssl_ssize_t send_size;
  wssl_chunk_chain_t* chunk_link;
  wssl_chunk_chain_t* chunk_link_next;
  wssl_chunk_t* chunk;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(chunk_link, chunk_link_next, &client->output_chunks)
  {
    chunk = wssl_chunk_chain_get_entry_from_client_chain_link(chunk_link);
    while(wssl_buffer_is_not_empty(&chunk->buffer))
    {
      if(client->ssl != WSSL_NULL)
      {
        int write_result = SSL_write
        (
          client->ssl,
          (void*)&chunk->buffer.data[chunk->buffer.begin],
          (int)(chunk->buffer.end-chunk->buffer.begin)
        );
        if(write_result <= 0)
        {
          int ssl_error = SSL_get_error(client->ssl, write_result);
          switch(ssl_error)
          {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
              client->ssl_want = ssl_error;
              TRY_CALL(wssl_client_sync_epoll_events(client));
              PASS;
              break;
            case SSL_ERROR_ZERO_RETURN:
              MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
              break;
            case SSL_ERROR_SYSCALL:
              if(write_result == 0)
                MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
              switch(errno)
              {
                case ECONNRESET:
                case EPIPE:
                  MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
                  break;
                default:
                  FAIL_ERRNO("SSL_write", errno);
                  break;
              }
              break;
            default:
              MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
              break;
          }
        }
        client->ssl_want = 0;
        send_size = (wssl_ssize_t)write_result;
      }
      else
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
              FAIL_ERRNO("send", errno);
              break;
          }
        if(send_size == 0)
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
      }

      chunk->buffer.begin += (wssl_size_t)send_size;
    }
    wssl_chunk_delete(chunk);
    client->number_of_output_chunks--;
  }

  TRY_CALL(wssl_client_sync_epoll_events(client));

  PASS;
}
