#ifndef _SERVER_H_
#define _SERVER_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_t
{
  wssl_server_chain_t wssl_chain_link;
  struct wssl_t*      wssl;
  char                ip[WSSL_IP_SIZE_IN_CHAR];
  int                 port;
  bool                ssl;
  bool                ipv6;
  int                 socket_descriptor;
  wssl_epoll_data_t   epoll_data;
  wssl_client_chain_t clients;
} wssl_server_t;

#define WSSL_NO_SSL  false
#define WSSL_SSL     true
#define WSSL_NO_IPV6 false
#define WSSL_IPV6    true

_INCLUDE_END_

MAKE_CHAIN_ENTRY(wssl_server, wssl_server_chain_t, wssl_server_t, wssl_chain_link)

#endif
