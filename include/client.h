#ifndef _CLIENT_H_
#define _CLIENT_H_

#define WSSL_CLIENT_STATE_TABLE(what_call)                            \
  what_call(WAIT_METHOD,                "Wait method"               ) \
  what_call(WAIT_URI_SEPARATOR,         "Wait URI separator"        ) \
  what_call(WAIT_URI,                   "Wait URI"                  ) \
  what_call(WAIT_VERSION_SEPARATOR,     "Wait version separator"    ) \
  what_call(WAIT_VERSION,               "Wait version"              ) \
  what_call(WAIT_CRLF,                  "Wait CRLF"                 ) \
  what_call(WAIT_FIELD_KEY,             "Wait field key"            ) \
  what_call(WAIT_FIELD_VALUE_SEPARATOR, "Wait field value separator") \
  what_call(WAIT_FIELD_VALUE,           "Wait field value"          ) \
  what_call(WAIT_FRAME,                 "Wait frame"                ) \
  what_call(WAIT_FIN_FRAME,             "Wait fin frame"            ) \
  what_call(TO_DELETE,                  "To delete"                 ) \

#define WSSL_CLIENT_DELETE_REASON_TABLE(what_call)    \
  what_call(NO_DELETE,         "No delete")           \
  what_call(FULL_RECV_BUFFER,  "Recv buffer is full") \
  what_call(DISCONNECTED,      "Disconnecte")         \
  what_call(CLOSED,            "Closed")              \
  what_call(MISSING_METHOD,    "Missing method")      \
  what_call(MISSING_URI,       "Missing URI")         \
  what_call(MISSING_VERSION,   "Missing URI")         \
  what_call(MISSING_CRLF,      "Missing CRLF")        \
  what_call(MISSING_FIELD_KEY, "Missing field key")   \
  what_call(BAD_HANDSHAKE,     "Bad handshake")       \
  what_call(BAD_FRAME_OPCODE,  "Bad frame opcode")    \
  what_call(STOPED,            "Stoped")              \

_INCLUDE_BEGIN_

typedef enum wssl_client_state_e
{
  #define CALL(what_name, what_string) WSSL_CLIENT_STATE_##what_name,
  WSSL_CLIENT_STATE_TABLE(CALL)
  #undef CALL
  WSSL_CLIENT_STATE_END_
} wssl_client_state_e;

typedef enum wssl_client_delete_reason_e
{
  #define CALL(what_name, what_string) WSSL_CLIENT_DELETE_REASON_##what_name,
  WSSL_CLIENT_DELETE_REASON_TABLE(CALL)
  #undef CALL
  WSSL_CLIENT_DELETE_REASON_END_
} wssl_client_delete_reason_e;

typedef struct wssl_client_t
{
  wssl_chain_t                chain_link;
  wssl_server_t*              server;
  struct wssl_t*              wssl;
  wssl_id_t                   id;
  int                         socket_descriptor;
  char                        ip[WSSL_IP_SIZE_IN_CHAR];
  int                         port;
  wssl_epoll_data_t           epoll_data;
  struct epoll_event          epoll_event;
  void*                       local_extra_data;
  wssl_buffer_t               input_buffer;
  wssl_buffer_t               output_buffer;
  wssl_client_state_e         state;
  wssl_client_delete_reason_e delete_reason;
  wssl_header_t               header;
  wssl_frame_t                frame;
} wssl_client_t;

static inline void wssl_client_set_local_extra_data
(
  _WSSL_MODIFY_ wssl_client_t* client,
  _WSSL_IN_     void*          local_extra_data
)
{
  client->local_extra_data = local_extra_data;
}

static inline bool wssl_client_is_to_delete
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->state == WSSL_CLIENT_STATE_TO_DELETE;
}

static inline bool wssl_client_is_not_to_delete
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->state != WSSL_CLIENT_STATE_TO_DELETE;
}

static inline void wssl_client_to_delete
(
  _WSSL_MODIFY_ wssl_client_t*              client,
  _WSSL_IN_     wssl_client_delete_reason_e delete_reason
)
{
  client->state = WSSL_CLIENT_STATE_TO_DELETE;
  client->delete_reason = delete_reason;
}

_INCLUDE_END_

#endif
