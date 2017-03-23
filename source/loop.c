#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_loop
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_ssize_t events_number;
  struct epoll_event events[EPOLL_EVENTS_SIZE];
  wssl_size_t event_index;
  wssl_epoll_data_t* epoll_data;
  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;

  wssl_generate_random_seed(wssl);

  wssl->epoll_descriptor = epoll_create1(0);
  if(wssl->epoll_descriptor < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_create1", errno);
  WSSL_CALL(wssl_servers_start(wssl));

  LOOP
  {
    events_number = (wssl_ssize_t)epoll_wait
    (
      wssl->epoll_descriptor,
      events,
      EPOLL_EVENTS_SIZE,
      (int)wssl->sleep_in_mseconds
    );
    if(events_number < 0)
      switch(errno)
      {
        case EINTR:
          break;
        default:
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_wait", errno);
          break;
      }
    else
      for(event_index = 0; event_index < events_number; event_index++)
      {
        epoll_data = (wssl_epoll_data_t*)events[event_index].data.ptr;
        switch(epoll_data->type)
        {
          case WSSL_EPOLL_DATA_TYPE_SERVER:
            WSSL_CALL(wssl_client_add(epoll_data->server));
            break;
          case WSSL_EPOLL_DATA_TYPE_CLIENT:
            if((events[event_index].events&EPOLLIN) != 0)
              WSSL_CALL(wssl_client_do_recv(epoll_data->client));
            if((events[event_index].events&EPOLLOUT) != 0)
              WSSL_CALL(wssl_client_do_send(epoll_data->client));
            break;
        }
      }

    if
    (
      wssl->tick_callback != WSSL_CALLBACK_NONE &&
      !(*wssl->tick_callback)(wssl)
    )
      break;

    WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_for_disconnecting)
      WSSL_CALL(wssl_client_delete(wssl_client_chain_entry(client_link)));
  }

  WSSL_CALL(wssl_servers_stop(wssl));
  if(close(wssl->epoll_descriptor) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "close", errno);
  wssl->epoll_descriptor = WSSL_NO_DESCRIPTOR;

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
