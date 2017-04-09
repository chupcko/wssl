#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_stop
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(epoll_ctl(server->wssl->epoll_descriptor, EPOLL_CTL_DEL, server->socket_descriptor, NULL) < 0)
    return MAKE_RESULT_ERRNO("epoll_ctl", errno);

  if(close(server->socket_descriptor) < 0)
    return MAKE_RESULT_ERRNO("close", errno);
  server->socket_descriptor = WSSL_NO_DESCRIPTOR;

  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  wssl_client_t* client;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &server->clients_in_wait_header)
  {
    client = wssl_client_chain_get_entry_from_chain_link(client_link);
    client->disconnect_reason = WSSL_CLIENT_DISCONNECT_REASON_STOPED;
    TRY_CALL(wssl_client_delete(client));
  }
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &server->clients_in_frame_processing)
  {
    client = wssl_client_chain_get_entry_from_chain_link(client_link);
    client->disconnect_reason = WSSL_CLIENT_DISCONNECT_REASON_STOPED;
    TRY_CALL(wssl_client_delete(client));
  }

  PASS;
}
