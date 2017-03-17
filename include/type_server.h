#ifndef _TYPE_SERVER_H_
#define _TYPE_SERVER_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_t
{
  wssl_chain_t   chain_link;
  char           ip[WSSL_IP_SIZE_IN_CHAR];
  int            port;
  int            socket_descriptor;
  wssl_epoll_t   epoll;
  wssl_chain_t   clients;
  struct wssl_t* wssl;
} wssl_server_t;

_INCLUDE_END_

#endif
