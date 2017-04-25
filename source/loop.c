#include "main.h"

static inline
wssl_result_t wssl_loop_init
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_generate_random_seed(wssl);

  wssl->epoll_descriptor = epoll_create1(0);
  if(wssl->epoll_descriptor < 0)
    FAIL_ERRNO("epoll_create1", errno);

  wssl_server_chain_t* server_link;
  CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    TRY_CALL(wssl_server_start(wssl_server_chain_get_entry_from_wssl_chain_link(server_link)));

  PASS;
}

static inline
wssl_result_t wssl_loop_clean
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_server_chain_t* server_link;
  CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    TRY_CALL(wssl_server_stop(wssl_server_chain_get_entry_from_wssl_chain_link(server_link)));

  if(close(wssl->epoll_descriptor) < 0)
    FAIL_ERRNO("close", errno);
  wssl->epoll_descriptor = WSSL_NO_DESCRIPTOR;

  PASS;
}

_LIBRARY_FUNCTION_
wssl_result_t wssl_loop
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_ssize_t events_number;
  struct epoll_event events[EPOLL_EVENTS_SIZE];
  wssl_ssize_t event_index;
  wssl_epoll_data_t* epoll_data;
  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;

  TRY_CALL(wssl_loop_init(wssl));

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
          FAIL_ERRNO("epoll_wait", errno);
          break;
      }
    else
      for(event_index = 0; event_index < events_number; event_index++)
      {
        epoll_data = (wssl_epoll_data_t*)events[event_index].data.ptr;
        switch(epoll_data->type)
        {
          case WSSL_EPOLL_DATA_TYPE_SERVER:
            TRY_CALL(wssl_client_add(epoll_data->server));
            break;
          case WSSL_EPOLL_DATA_TYPE_CLIENT:
            if((events[event_index].events&EPOLLIN) != 0)
              TRY_CALL(wssl_client_do_recv(epoll_data->client));
            if((events[event_index].events&EPOLLOUT) != 0)
              TRY_CALL(wssl_client_do_send(epoll_data->client));
            break;
        }
      }

    if
    (
      wssl->on_tick_callback != WSSL_CALLBACK_NONE &&
      !(*wssl->on_tick_callback)(wssl)
    )
      break;

    CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_marked_for_disconnecting)
      TRY_CALL(wssl_client_delete(wssl_client_chain_get_entry_from_wssl_chain_link(client_link)));
  }

  TRY_CALL(wssl_loop_clean(wssl));

  PASS;
}
