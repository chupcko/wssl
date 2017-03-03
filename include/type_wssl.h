#ifndef _TYPE_WSSL_H_
#define _TYPE_WSSL_H_

_INCLUDE_BEGIN_

typedef struct wssl_t
{
  wssl_chain_t                         servers;
  wssl_chain_t                         epolls;
  int                                  epoll_descriptor;
  void*                                global_extra_data;
  wssl_connect_callback_function_t*    connect_callback_function;
  wssl_disconnect_callback_function_t* disconnect_callback_function;
  wssl_receiving_callback_function_t*  receiving_callback_function;
  wssl_tick_callback_function_t*       tick_callback_function;
} wssl_t;

#define WSSL_INIT_VALUE(name)                                          \
{                                                                      \
  .servers                      = WSSL_CHAIN_INIT_VALUE(name.servers), \
  .epolls                       = WSSL_CHAIN_INIT_VALUE(name.epolls),  \
  .epoll_descriptor             = WSSL_NO_DESCRIPTOR,                  \
  .global_extra_data            = WSSL_NULL,                           \
  .connect_callback_function    = WSSL_CALLBACK_FUNCTION_NONE,         \
  .disconnect_callback_function = WSSL_CALLBACK_FUNCTION_NONE,         \
  .receiving_callback_function  = WSSL_CALLBACK_FUNCTION_NONE,         \
  .tick_callback_function       = WSSL_CALLBACK_FUNCTION_NONE          \
}                                                                      \

#define WSSL_DECLARE(name) wssl_t name = WSSL_INIT_VALUE(name)

static inline void wssl_init(wssl_t* wssl)
{
  wssl_chain_init(&wssl->servers);
  wssl_chain_init(&wssl->epolls);
  wssl->epoll_descriptor             = WSSL_NO_DESCRIPTOR;
  wssl->global_extra_data            = WSSL_NULL;
  wssl->connect_callback_function    = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->disconnect_callback_function = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->receiving_callback_function  = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->tick_callback_function       = WSSL_CALLBACK_FUNCTION_NONE;
}

_INCLUDE_END_

#endif
