#ifndef _INLINE_H_
#define _INLINE_H_

static inline wssl_result_t wssl_client_epoll_event_add_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events |= EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}

static inline wssl_result_t wssl_client_epoll_event_delete_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events &= ~EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}

#endif
