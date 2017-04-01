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
  void*                           connection_extra_data;
  wssl_buffer_t                   input_buffer;
  wssl_chunk_chain_t              output_chunks;
  wssl_client_state_e             state;
  wssl_client_disconnect_reason_e disconnect_reason;
  wssl_header_t                   header;
  wssl_frame_t                    frame;
} wssl_client_t;

static inline
void wssl_client_set_connection_extra_data
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     void*          connection_extra_data
)
{
  client->connection_extra_data = connection_extra_data;
}

static inline
void* wssl_client_get_connection_extra_data
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->connection_extra_data;
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

static inline
bool wssl_client_disconnect_reason_is_error
(
  _WSSL_IN_ const wssl_client_disconnect_reason_e disconnect_reason
)
{
  return
    disconnect_reason != WSSL_CLIENT_DISCONNECT_REASON_NONE &&
    disconnect_reason != WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED &&
    disconnect_reason != WSSL_CLIENT_DISCONNECT_REASON_CLOSED &&
    disconnect_reason != WSSL_CLIENT_DISCONNECT_REASON_REQUESTED &&
    disconnect_reason != WSSL_CLIENT_DISCONNECT_REASON_STOPED;
}

static inline
bool wssl_client_disconnect_reason_is_not_error
(
  _WSSL_IN_ const wssl_client_disconnect_reason_e disconnect_reason
)
{
  return
    disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_NONE ||
    disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_DISCONNECTED ||
    disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_CLOSED ||
    disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_REQUESTED ||
    disconnect_reason == WSSL_CLIENT_DISCONNECT_REASON_STOPED;
}

static inline
void wssl_client_print
(
  _WSSL_IN_     const wssl_client_t* client,
  _WSSL_MODIFY_       FILE*          file
)
{
  wssl_id_print(&client->id, file);
  fprintf
  (
    file,
    " %s:%d -> %s:%d",
    client->ip,
    client->port,
    client->server->ip,
    client->server->port
  );
}

_INCLUDE_END_

MAKE_CHAIN_ENTRY(wssl_client, wssl_client_chain_t, wssl_client_t, chain_link)

#endif
