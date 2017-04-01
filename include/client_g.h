#ifndef _CLIENT_G_H_
#define _CLIENT_G_H_

#define CLIENT_STATE_TABLE(what_call)                                 \
  what_call(WAIT_METHOD,                "Wait method"               ) \
  what_call(WAIT_URI_SEPARATOR,         "Wait URI separator"        ) \
  what_call(WAIT_URI,                   "Wait URI"                  ) \
  what_call(WAIT_VERSION_SEPARATOR,     "Wait version separator"    ) \
  what_call(WAIT_VERSION,               "Wait version"              ) \
  what_call(WAIT_CRLF,                  "Wait CRLF"                 ) \
  what_call(WAIT_FIELD_KEY,             "Wait field key"            ) \
  what_call(WAIT_FIELD_VALUE_SEPARATOR, "Wait field value separator") \
  what_call(WAIT_FIELD_VALUE,           "Wait field value"          ) \
  what_call(PROCESSING_HEADER,          "Processing header"         ) \
  what_call(WAIT_FRAME,                 "Wait frame"                ) \
  what_call(WAIT_FIN_FRAME,             "Wait fin frame"            ) \
  what_call(FOR_DISCONNECTING,          "For disconnecting"         ) \

#define CLIENT_DISCONNECT_REASON_TABLE(what_call)      \
  what_call(NONE,              "None")                 \
  what_call(FULL_RECV_BUFFER,  "Recv buffer is full")  \
  what_call(DISCONNECTED,      "Disconnecte")          \
  what_call(CLOSED,            "Closed")               \
  what_call(MISSING_METHOD,    "Missing method")       \
  what_call(MISSING_URI,       "Missing URI")          \
  what_call(MISSING_VERSION,   "Missing URI")          \
  what_call(MISSING_CRLF,      "Missing CRLF")         \
  what_call(MISSING_FIELD_KEY, "Missing field key")    \
  what_call(BAD_HANDSHAKE,     "Bad handshake")        \
  what_call(BAD_FRAME_OPCODE,  "Bad frame opcode")     \
  what_call(REQUESTED,         "Requested")            \
  what_call(STOPED,            "Stoped")               \

_INCLUDE_BEGIN_

typedef enum wssl_client_state_e
{
  #define MACRO_CALL(what_id, what_name) WSSL_CLIENT_STATE_##what_id,
  CLIENT_STATE_TABLE(MACRO_CALL)
  #undef MACRO_CALL
  WSSL_CLIENT_STATE_END_
} wssl_client_state_e;

typedef enum wssl_client_disconnect_reason_e
{
  #define MACRO_CALL(what_id, what_name) WSSL_CLIENT_DISCONNECT_REASON_##what_id,
  CLIENT_DISCONNECT_REASON_TABLE(MACRO_CALL)
  #undef MACRO_CALL
  WSSL_CLIENT_DISCONNECT_REASON_END_
} wssl_client_disconnect_reason_e;

_INCLUDE_END_

#endif
