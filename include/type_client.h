#ifndef _TYPE_CLIENT_H_
#define _TYPE_CLIENT_H_

_INCLUDE_BEGIN_

typedef struct wssl_client_t
{
  wssl_chain_t           chain_link;
  wssl_id_t              id;
  int                    socket_descriptor;
  char                   ip[WSSL_IP_SIZE];
  int                    port;
  wssl_epoll_t           epoll;
  wssl_buffer_t          input_buffer;
  wssl_buffer_t          output_buffer;
  struct wssl_t*         wssl;
  struct wssl_server_t*  server;
  void*                  local_extra_data;
  wssl_header_t          header;
} wssl_client_t;

static inline void wssl_set_local_extra_data
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     void*          local_extra_data
)
{
  client->local_extra_data = local_extra_data;
}

_INCLUDE_END_

#endif
