#ifndef _WSSL_H_
#define _WSSL_H_

_INCLUDE_BEGIN_

typedef struct wssl_t
{
  int                                   epoll_descriptor;
  char*                                 server_name;
  wssl_size_t                           buffer_size_in_octets;
  wssl_ssize_t                          sleep_in_mseconds;
  void*                                 global_extra_data;
  wssl_connect_callback_f*              connect_callback;
  wssl_disconnect_callback_f*           disconnect_callback;
  wssl_header_callback_f*               header_callback;
  wssl_begin_callback_f*                begin_callback;
  wssl_receive_text_frame_callback_f*   receive_text_frame_callback;
  wssl_receive_binary_frame_callback_f* receive_binary_frame_callback;
  wssl_receive_close_frame_callback_f*  receive_close_frame_callback;
  wssl_receive_ping_frame_callback_f*   receive_ping_frame_callback;
  wssl_receive_pong_frame_callback_f*   receive_pong_frame_callback;
  wssl_tick_callback_f*                 tick_callback;
  unsigned int                          random_seed;
  wssl_id_t                             next_client_id;
  wssl_server_chain_t                   servers;
  wssl_client_chain_t                   clients_for_disconnecting;
} wssl_t;

#define WSSL_INIT_VALUE(what_name)                                                            \
{                                                                                             \
  .epoll_descriptor              = WSSL_NO_DESCRIPTOR,                                        \
  .server_name                   = WSSL_DEFAULT_SERVER_NAME,                                  \
  .buffer_size_in_octets         = WSSL_DEFAULT_BUFFER_SIZE_IN_OCTETS,                        \
  .sleep_in_mseconds             = WSSL_DEFAULT_SLEEP_IN_MSECONDS,                            \
  .global_extra_data             = WSSL_NULL,                                                 \
  .connect_callback              = WSSL_CALLBACK_NONE,                                        \
  .disconnect_callback           = WSSL_CALLBACK_NONE,                                        \
  .header_callback               = WSSL_CALLBACK_NONE,                                        \
  .begin_callback                = WSSL_CALLBACK_NONE,                                        \
  .receive_text_frame_callback   = WSSL_CALLBACK_NONE,                                        \
  .receive_binary_frame_callback = WSSL_CALLBACK_NONE,                                        \
  .receive_close_frame_callback  = WSSL_CALLBACK_NONE,                                        \
  .receive_ping_frame_callback   = WSSL_CALLBACK_NONE,                                        \
  .receive_pong_frame_callback   = WSSL_CALLBACK_NONE,                                        \
  .tick_callback                 = WSSL_CALLBACK_NONE,                                        \
  .random_seed                   = 0,                                                         \
  .next_client_id                = WSSL_ID_INIT_VALUE,                                        \
  .servers                       = WSSL_CHAIN_INIT_VALUE(what_name.servers),                  \
  .clients_for_disconnecting     = WSSL_CHAIN_INIT_VALUE(what_name.clients_for_disconnecting) \
}                                                                                             \

#define WSSL_DECLARE(what_name) wssl_t what_name = WSSL_INIT_VALUE(what_name)

static inline
void wssl_init
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl->epoll_descriptor              = WSSL_NO_DESCRIPTOR;
  wssl->server_name                   = WSSL_DEFAULT_SERVER_NAME;
  wssl->buffer_size_in_octets         = WSSL_DEFAULT_BUFFER_SIZE_IN_OCTETS,
  wssl->sleep_in_mseconds             = WSSL_DEFAULT_SLEEP_IN_MSECONDS,
  wssl->global_extra_data             = WSSL_NULL;
  wssl->connect_callback              = WSSL_CALLBACK_NONE;
  wssl->disconnect_callback           = WSSL_CALLBACK_NONE;
  wssl->header_callback               = WSSL_CALLBACK_NONE;
  wssl->begin_callback                = WSSL_CALLBACK_NONE;
  wssl->receive_text_frame_callback   = WSSL_CALLBACK_NONE;
  wssl->receive_binary_frame_callback = WSSL_CALLBACK_NONE;
  wssl->receive_close_frame_callback  = WSSL_CALLBACK_NONE;
  wssl->receive_ping_frame_callback   = WSSL_CALLBACK_NONE;
  wssl->receive_pong_frame_callback   = WSSL_CALLBACK_NONE;
  wssl->tick_callback                 = WSSL_CALLBACK_NONE;
  wssl->random_seed                   = 0;
  wssl_id_init(&wssl->next_client_id);
  wssl_server_chain_init(&wssl->servers);
  wssl_client_chain_init(&wssl->clients_for_disconnecting);
}

static inline
void* wssl_get_global_extra_data
(
  _WSSL_IN_ const wssl_t* wssl
)
{
  return wssl->global_extra_data;
}

static inline
void wssl_set_id_prefix
(
  _WSSL_MODIFY_       wssl_t*          wssl,
  _WSSL_IN_     const wssl_id_prefix_t prefix
)
{
  wssl_id_set_prefix(&wssl->next_client_id, prefix);
}

static inline
void wssl_set_id_suffix
(
  _WSSL_MODIFY_       wssl_t*          wssl,
  _WSSL_IN_     const wssl_id_suffix_t suffix
)
{
  wssl_id_set_prefix(&wssl->next_client_id, suffix);
}

_INCLUDE_END_

static inline
void wssl_generate_random_seed
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl->random_seed = (unsigned int)time(NULL);
}

static inline
void wssl_set_next_client_id
(
  _WSSL_MODIFY_ wssl_t*    wssl,
  _WSSL_OUT_    wssl_id_t* id
)
{
  *id = wssl->next_client_id;
  wssl_id_next(&wssl->next_client_id);
}

#endif
