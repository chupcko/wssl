#ifndef _TYPE_SERVER_H_
#define _TYPE_SERVER_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_t
{
  wssl_chain_t link;
  char         ip[WSSL_IP_SIZE];
  int          port;
  int          socket_descriptor;
  wssl_epoll_t epoll;
} wssl_server_t;

_INCLUDE_END_

#endif
