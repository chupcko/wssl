#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_loop
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  struct epoll_event events[EPOLL_EVENTS_SIZE];
  int events_number;
  int i;
  wssl_epoll_t* epoll;

  wssl->epoll_descriptor = epoll_create1(0);
  if(wssl->epoll_descriptor < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_create1", errno);
  WSSL_CALL(wssl_servers_start(wssl));

  LOOP
  {
    events_number = epoll_wait(wssl->epoll_descriptor, events, EPOLL_EVENTS_SIZE, EPOLL_SLEEP_MSECONDS);
    if(events_number < 0 && errno != EINTR)
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_wait", errno);
    for(i = 0; i < events_number; i++)
    {
      epoll = (wssl_epoll_t*)events[i].data.ptr;
      switch(epoll->type)
      {
        case WSSL_EPOLL_TYPE_SERVER:
          WSSL_CALL(wssl_client_add(wssl, epoll->server));
          break;
        case WSSL_EPOLL_TYPE_CLIENT:
          WSSL_CALL(wssl_client_do(wssl, epoll->client));
          break;
      }
    }

    if
    (
      wssl->tick_callback_function != WSSL_CALLBACK_FUNCTION_NONE &&
      !(*wssl->tick_callback_function)(wssl)
    )
      break;
  }

  WSSL_CALL(wssl_servers_stop(wssl));
  if(close(wssl->epoll_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "close", errno);
  wssl->epoll_descriptor = WSSL_NO_DESCRIPTOR;

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
