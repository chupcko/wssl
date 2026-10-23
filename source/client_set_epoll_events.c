#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_set_epoll_events
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     uint32_t       events
)
{
  if(client->epoll_event.events != events)
  {
    client->epoll_event.events = events;
    if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
      FAIL_ERRNO("epoll_ctl", errno);
  }

  PASS;
}
