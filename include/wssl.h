#ifndef _WSSL_H_
#define _WSSL_H_

_INCLUDE_BEGIN_

typedef struct wssl_t
{
  char*                                      server_name;
  wssl_size_t                                buffer_size_in_octets;
  wssl_ssize_t                               sleep_in_mseconds;
  wssl_size_t                                max_number_of_output_chunks;
  wssl_size_t                                max_number_of_received_multi_frames;
  int                                        epoll_descriptor;
  void*                                      global_extra_data;
  wssl_on_connect_callback_f*                on_connect_callback;
  wssl_on_disconnect_callback_f*             on_disconnect_callback;
  wssl_on_receive_header_callback_f*         on_receive_header_callback;
  wssl_on_start_receiving_frames_callback_f* on_start_receiving_frames_callback;
  wssl_on_receive_text_frame_callback_f*     on_receive_text_frame_callback;
  wssl_on_receive_binary_frame_callback_f*   on_receive_binary_frame_callback;
  wssl_on_receive_close_frame_callback_f*    on_receive_close_frame_callback;
  wssl_on_receive_ping_frame_callback_f*     on_receive_ping_frame_callback;
  wssl_on_receive_pong_frame_callback_f*     on_receive_pong_frame_callback;
  wssl_on_tick_callback_f*                   on_tick_callback;
  unsigned int                               random_seed;
  wssl_id_t                                  next_client_id;
  wssl_server_chain_t                        servers;
  wssl_client_chain_t                        clients_marked_for_disconnecting;
} wssl_t;

#define WSSL_INIT_VALUE(what_name)                                                                              \
{                                                                                                               \
  .server_name                         = WSSL_DEFAULT_SERVER_NAME,                                              \
  .buffer_size_in_octets               = WSSL_DEFAULT_BUFFER_SIZE_IN_OCTETS,                                    \
  .sleep_in_mseconds                   = WSSL_DEFAULT_SLEEP_IN_MSECONDS,                                        \
  .max_number_of_output_chunks         = WSSL_DEFAULT_MAX_NUMBER_OF_OUTPUT_CHUNKS,                              \
  .max_number_of_received_multi_frames = WSSL_DEFAULT_MAX_NUMBER_OF_RECEIVED_MULTI_FRAMES,                      \
  .epoll_descriptor                    = WSSL_NO_DESCRIPTOR,                                                    \
  .global_extra_data                   = WSSL_NULL,                                                             \
  .on_connect_callback                 = WSSL_CALLBACK_NONE,                                                    \
  .on_disconnect_callback              = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_header_callback          = WSSL_CALLBACK_NONE,                                                    \
  .on_start_receiving_frames_callback  = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_text_frame_callback      = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_binary_frame_callback    = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_close_frame_callback     = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_ping_frame_callback      = WSSL_CALLBACK_NONE,                                                    \
  .on_receive_pong_frame_callback      = WSSL_CALLBACK_NONE,                                                    \
  .on_tick_callback                    = WSSL_CALLBACK_NONE,                                                    \
  .random_seed                         = 0,                                                                     \
  .next_client_id                      = WSSL_ID_INIT_VALUE,                                                    \
  .servers                             = WSSL_CHAIN_ROOT_INIT_VALUE(what_name.servers),                         \
  .clients_marked_for_disconnecting    = WSSL_CHAIN_ROOT_INIT_VALUE(what_name.clients_marked_for_disconnecting) \
}                                                                                                               \

#define WSSL_DECLARE(what_name) wssl_t what_name = WSSL_INIT_VALUE(what_name)

static inline
void wssl_init
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl->server_name                         = WSSL_DEFAULT_SERVER_NAME;
  wssl->buffer_size_in_octets               = WSSL_DEFAULT_BUFFER_SIZE_IN_OCTETS,
  wssl->sleep_in_mseconds                   = WSSL_DEFAULT_SLEEP_IN_MSECONDS,
  wssl->max_number_of_output_chunks         = WSSL_DEFAULT_MAX_NUMBER_OF_OUTPUT_CHUNKS,
  wssl->max_number_of_received_multi_frames = WSSL_DEFAULT_MAX_NUMBER_OF_RECEIVED_MULTI_FRAMES,
  wssl->epoll_descriptor                    = WSSL_NO_DESCRIPTOR;
  wssl->global_extra_data                   = WSSL_NULL;
  wssl->on_connect_callback                 = WSSL_CALLBACK_NONE;
  wssl->on_disconnect_callback              = WSSL_CALLBACK_NONE;
  wssl->on_receive_header_callback          = WSSL_CALLBACK_NONE;
  wssl->on_start_receiving_frames_callback  = WSSL_CALLBACK_NONE;
  wssl->on_receive_text_frame_callback      = WSSL_CALLBACK_NONE;
  wssl->on_receive_binary_frame_callback    = WSSL_CALLBACK_NONE;
  wssl->on_receive_close_frame_callback     = WSSL_CALLBACK_NONE;
  wssl->on_receive_ping_frame_callback      = WSSL_CALLBACK_NONE;
  wssl->on_receive_pong_frame_callback      = WSSL_CALLBACK_NONE;
  wssl->on_tick_callback                    = WSSL_CALLBACK_NONE;
  wssl->random_seed                         = 0;
  wssl_id_init(&wssl->next_client_id);
  wssl_server_chain_root_init(&wssl->servers);
  wssl_client_chain_root_init(&wssl->clients_marked_for_disconnecting);
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
