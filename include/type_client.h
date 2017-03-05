#ifndef _TYPE_CLIENT_H_
#define _TYPE_CLIENT_H_

_INCLUDE_BEGIN_

typedef struct wssl_client_t
{
  wssl_chain_t           chain_link;
  int                    socket_descriptor;
  char                   ip[WSSL_IP_SIZE];
  int                    port;
  wssl_connection_info_t connection_info;
  wssl_epoll_t           epoll;
} wssl_client_t;

_INCLUDE_END_

#endif
