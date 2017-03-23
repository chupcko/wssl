#ifndef _REST_H_
#define _REST_H_

static inline const char* wssl_get_bool_string
(
  _WSSL_IN_ const bool value
)
{
  if(value)
    return "true";
  return "false";
}

static inline void wssl_client_set_for_disconnecting
(
  _WSSL_MODIFY_ wssl_client_t*                  client,
  _WSSL_IN_     wssl_client_disconnect_reason_e disconnect_reason
)
{
  client->state = WSSL_CLIENT_STATE_FOR_DISCONNECTING;
  client->disconnect_reason = disconnect_reason;
  wssl_client_chain_delete_link(&client->chain_link);
  wssl_client_chain_add_link_backward(&client->wssl->clients_for_disconnecting, &client->chain_link);
}

static inline wssl_result_t wssl_client_epoll_event_add_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events |= EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline wssl_result_t wssl_client_epoll_event_delete_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events &= ~EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

#endif
