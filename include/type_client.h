#ifndef _TYPE_CLIENT_H_
#define _TYPE_CLIENT_H_

#define WSSL_CLIENT_STATE_TABLE(what_call)                            \
  what_call(WAIT_METHOD,                "Wait method"               ) \
  what_call(WAIT_URI_SEPARATOR,         "Wait URI separator"        ) \
  what_call(WAIT_URI,                   "Wait uri"                  ) \
  what_call(WAIT_VERSION_SEPARATOR,     "Wait version separator"    ) \
  what_call(WAIT_VERSION,               "Wait version"              ) \
  what_call(WAIT_CRLF,                  "Wait CRLF"                 ) \
  what_call(WAIT_FIELD_KEY,             "Wait field key"            ) \
  what_call(WAIT_FIELD_VALUE_SEPARATOR, "Wait field value separator") \
  what_call(WAIT_FIELD_VALUE,           "Wait field value"          ) \
  what_call(WAIT_FRAME,                 "Wait frame"                ) \
  what_call(DELETE,                     "Delete"                    ) \

_INCLUDE_BEGIN_

typedef enum wssl_client_state_e
{
  #define CALL(what_name, what_string) WSSL_CLIENT_STATE_##what_name,
  WSSL_CLIENT_STATE_TABLE(CALL)
  #undef CALL
  WSSL_CLIENT_STATE_END_
} wssl_client_state_e;

typedef struct wssl_client_t
{
  wssl_chain_t          chain_link;
  wssl_id_t             id;
  int                   socket_descriptor;
  char                  ip[WSSL_IP_SIZE_IN_CHAR];
  int                   port;
  wssl_epoll_t          epoll;
  void*                 local_extra_data;
  wssl_buffer_t         input_buffer;
  wssl_buffer_t         output_buffer;
  wssl_client_state_e   state;
  wssl_header_t         header;
  struct wssl_t*        wssl;
  struct wssl_server_t* server;
} wssl_client_t;

static inline void wssl_set_local_extra_data
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     void*          local_extra_data
)
{
  client->local_extra_data = local_extra_data;
}

_INCLUDE_END_

#endif
