#ifndef _TYPE_EPOLL_H_
#define _TYPE_EPOLL_H_

_INCLUDE_BEGIN_

typedef enum wssl_epoll_type_e
{
  WSSL_EPOLL_TYPE_SERVER,
  WSSL_EPOLL_TYPE_CLIENT
} wssl_epoll_type_e;

typedef struct wssl_epoll_t
{
  wssl_epoll_type_e type;
  union
  {
    struct wssl_server_t* server;
    struct wssl_client_t* client;
  };
} wssl_epoll_t;

_INCLUDE_END_

#endif
