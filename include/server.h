#ifndef _SERVER_H_
#define _SERVER_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_t
{
  wssl_chain_t      chain_link;
  struct wssl_t*    wssl;
  char              ip[WSSL_IP_SIZE_IN_CHAR];
  int               port;
  int               socket_descriptor;
  wssl_epoll_data_t epoll_data;
  wssl_chain_t      clients;
} wssl_server_t;

_INCLUDE_END_

#endif
