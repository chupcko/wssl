#ifndef _SERVER_H_
#define _SERVER_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_t
{
  wssl_server_chain_t         wssl_chain_link;
  struct wssl_t*              wssl;
  wssl_server_configuration_t configuration;
  int                         socket_descriptor;
  struct ssl_ctx_st*          ssl_ctx;
  wssl_epoll_data_t           epoll_data;
  wssl_client_chain_t         clients;
} wssl_server_t;

_INCLUDE_END_

MAKE_CHAIN_ENTRY(wssl_server, wssl_server_chain_t, wssl_server_t, wssl_chain_link)

#endif
