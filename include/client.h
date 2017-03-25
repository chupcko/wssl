#ifndef _CLIENT_H_
#define _CLIENT_H_

_INCLUDE_BEGIN_

typedef struct wssl_client_t
{
  wssl_client_chain_t             chain_link;
  wssl_server_t*                  server;
  struct wssl_t*                  wssl;
  wssl_id_t                       id;
  int                             socket_descriptor;
  char                            ip[WSSL_IP_SIZE_IN_CHAR];
  int                             port;
  wssl_epoll_data_t               epoll_data;
  struct epoll_event              epoll_event;
  void*                           local_extra_data;
  wssl_buffer_t                   input_buffer;
  wssl_chunk_chain_t              output_chunks;
  wssl_client_state_e             state;
  wssl_client_disconnect_reason_e disconnect_reason;
  wssl_header_t                   header;
  wssl_frame_t                    frame;
} wssl_client_t;

static inline
void wssl_client_set_local_extra_data
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     void*          local_extra_data
)
{
  client->local_extra_data = local_extra_data;
}

static inline
void* wssl_client_get_local_extra_data
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->local_extra_data;
}

static inline
bool wssl_client_is_for_disconnecting
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->state == WSSL_CLIENT_STATE_FOR_DISCONNECTING;
}

static inline
bool wssl_client_is_not_for_disconnecting
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->state != WSSL_CLIENT_STATE_FOR_DISCONNECTING;
}

static inline
bool wssl_client_is_in_frame_processing
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return
    client->state == WSSL_CLIENT_STATE_WAIT_FRAME ||
    client->state == WSSL_CLIENT_STATE_WAIT_FIN_FRAME;
}

static inline
bool wssl_client_is_not_in_frame_processing
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return
    client->state != WSSL_CLIENT_STATE_WAIT_FRAME &&
    client->state != WSSL_CLIENT_STATE_WAIT_FIN_FRAME;
}

_INCLUDE_END_

#endif
