#ifndef _TYPE_H_
#define _TYPE_H_

_INCLUDE_BEGIN_

typedef uint8_t wssl_octet_t;
typedef size_t wssl_size_t;
typedef ssize_t wssl_ssize_t;

#define WSSL_IP_SIZE ((3+1)*4)

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

typedef void wssl_connect_callback_function_t
(
  _WSSL_MODIFY_ wssl_connection_info_t*
);
typedef void wssl_disconnect_callback_function_t
(
  _WSSL_MODIFY_ wssl_connection_info_t*
);
typedef bool wssl_receiving_callback_function_t
(
  _WSSL_MODIFY_ wssl_connection_info_t*,
  _WSSL_IN_     wssl_octet_t*,
  _WSSL_IN_     wssl_size_t
);
typedef bool wssl_socket_event_callback_function_t
(
  _WSSL_MODIFY_ wssl_connection_info_t*,
  _WSSL_IN_     int
);
typedef bool wssl_tick_callback_function_t
(
  _WSSL_MODIFY_ wssl_connection_info_t*
);

#define WSSL_CALLBACK_FUNCTION_NONE NULL

typedef struct wssl_t
{
  void*                                  global_extra_data;
  wssl_connect_callback_function_t*      connect_callback_function;
  wssl_disconnect_callback_function_t*   disconnect_callback_function;
  wssl_receiving_callback_function_t*    receiving_callback_function;
  wssl_socket_event_callback_function_t* socket_event_callback_function;
  wssl_tick_callback_function_t*         tick_callback_function;
} wssl_t;

_INCLUDE_END_

#endif
