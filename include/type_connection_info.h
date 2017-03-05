#ifndef _TYPE_CONNECTION_INFO_H_
#define _TYPE_CONNECTION_INFO_H_

_INCLUDE_BEGIN_

typedef struct wssl_connection_info_t
{
  struct wssl_t*        wssl;
  struct wssl_server_t* server;
  struct wssl_client_t* client;
  void*                 local_extra_data;
} wssl_connection_info_t;

_INCLUDE_END_

#endif
