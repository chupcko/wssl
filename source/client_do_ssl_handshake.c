#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do_ssl_handshake
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);

  if(client->ssl_handshake_done)
    PASS;

  int accept_result = SSL_accept(client->ssl);
  if(accept_result == 1)
  {
    client->ssl_handshake_done = true;
    client->ssl_want = 0;
    TRY_CALL(wssl_client_sync_epoll_events(client));
    PASS;
  }

  int ssl_error = SSL_get_error(client->ssl, accept_result);
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
      if(accept_result == 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_CLOSED);
      switch(errno)
      {
        case ECONNRESET:
        case EPIPE:
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
          break;
        default:
          FAIL_ERRNO("SSL_accept", errno);
          break;
      }
      break;
    default:
      MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED);
      break;
  }
}
