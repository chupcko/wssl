#ifndef _TYPE_WSSL_H_
#define _TYPE_WSSL_H_

_INCLUDE_BEGIN_

typedef struct wssl_t
{
  int                                  epoll_descriptor;
  void*                                global_extra_data;
  wssl_connect_callback_function_t*    connect_callback_function;
  wssl_disconnect_callback_function_t* disconnect_callback_function;
  wssl_receiving_callback_function_t*  receiving_callback_function;
  wssl_tick_callback_function_t*       tick_callback_function;
  wssl_id_t                            next_client_id;
  wssl_chain_t                         servers;
} wssl_t;

#define WSSL_INIT_VALUE(what_name)                                         \
{                                                                          \
  .epoll_descriptor             = WSSL_NO_DESCRIPTOR,                      \
  .global_extra_data            = WSSL_NULL,                               \
  .connect_callback_function    = WSSL_CALLBACK_FUNCTION_NONE,             \
  .disconnect_callback_function = WSSL_CALLBACK_FUNCTION_NONE,             \
  .receiving_callback_function  = WSSL_CALLBACK_FUNCTION_NONE,             \
  .tick_callback_function       = WSSL_CALLBACK_FUNCTION_NONE,             \
  .next_client_id               = WSSL_ID_INIT_VALUE,                      \
  .servers                      = WSSL_CHAIN_INIT_VALUE(what_name.servers) \
}                                                                          \

#define WSSL_DECLARE(what_name) wssl_t what_name = WSSL_INIT_VALUE(what_name)

static inline void wssl_init
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl->epoll_descriptor             = WSSL_NO_DESCRIPTOR;
  wssl->global_extra_data            = WSSL_NULL;
  wssl->connect_callback_function    = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->disconnect_callback_function = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->receiving_callback_function  = WSSL_CALLBACK_FUNCTION_NONE;
  wssl->tick_callback_function       = WSSL_CALLBACK_FUNCTION_NONE;
  wssl_id_init(&wssl->next_client_id);
  wssl_chain_init(&wssl->servers);
}

static inline void wssl_set_global_extra_data
(
  _WSSL_MODIFY_ wssl_t* wssl,
  _WSSL_IN_     void*   global_extra_data
)
{
  wssl->global_extra_data = global_extra_data;
}

static inline void wssl_set_connect_callback_function
(
  _WSSL_MODIFY_ wssl_t*                           wssl,
  _WSSL_IN_     wssl_connect_callback_function_t* connect_callback_function
)
{
  wssl->connect_callback_function = connect_callback_function;
}

static inline void wssl_set_disconnect_callback_function
(
  _WSSL_MODIFY_ wssl_t*                              wssl,
  _WSSL_IN_     wssl_disconnect_callback_function_t* disconnect_callback_function
)
{
  wssl->disconnect_callback_function = disconnect_callback_function;
}

static inline void wssl_set_receiving_callback_function
(
  _WSSL_MODIFY_ wssl_t*                             wssl,
  _WSSL_IN_     wssl_receiving_callback_function_t* receiving_callback_function
)
{
  wssl->receiving_callback_function = receiving_callback_function;
}

static inline void wssl_set_tick_callback_function
(
  _WSSL_MODIFY_ wssl_t*                        wssl,
  _WSSL_IN_     wssl_tick_callback_function_t* tick_callback_function
)
{
  wssl->tick_callback_function = tick_callback_function;
}

_INCLUDE_END_

static inline wssl_id_t wssl_get_next_client_id
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  wssl_id_t id = wssl->next_client_id;
  wssl_id_next(&wssl->next_client_id);
  return id;
}

#endif
