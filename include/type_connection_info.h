#ifndef _TYPE_CONNECTION_INFO_H_
#define _TYPE_CONNECTION_INFO_H_

_INCLUDE_BEGIN_

typedef struct wssl_connection_info_t
{
  int   socket_file_descriptor;
  char  server_ip[WSSL_IP_SIZE];
  int   server_port;
  char  client_ip[WSSL_IP_SIZE];
  int   client_port;
  void* global_extra_data;
  void* local_extra_data;
} wssl_connection_info_t;

_INCLUDE_END_

#endif
