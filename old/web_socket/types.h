#ifndef _TYPES_H_
#define _TYPES_H_

#define BITS_IN_BYTE 8

typedef enum header_key_index_t
{
  HEADER_INDEX_GET,
  HEADER_INDEX_HOST,
  HEADER_INDEX_UPGRADE,
  HEADER_INDEX_SEC_WS_KEY,
  HEADER_INDEX_END,
  HEADER_INDEX_SIZE
} header_key_index_t;

#define HEADER_NAME_GET        "GET "
#define HEADER_NAME_HOST       "Host: "
#define HEADER_NAME_UPGRADE    "Upgrade: "
#define HEADER_NAME_SEC_WS_KEY "Sec-WebSocket-Key: "
#define HEADER_NAME_END        ""

typedef struct header_key_t
{
  char* name;
  int name_length;
} header_key_t;

typedef struct header_data_t
{
  char* value;
  int value_length;
} header_data_t;

#define FRAME_MASKING_KEY_SIZE 4
typedef struct frame_t
{
  bool fin;
  uint8_t opcode;
  bool masked;
  uint8_t payload_size;
  uint64_t payload_length;
  uint8_t masking_key[FRAME_MASKING_KEY_SIZE];
  int payload_offset;
  uint8_t* payload;
} frame_t;

#define FRAME_OPCODE_CONTINUE 0x0
#define FRAME_OPCODE_TEXT     0x1
#define FRAME_OPCODE_BINARY   0x2
#define FRAME_OPCODE_CLOSE    0x8
#define FRAME_OPCODE_PING     0x9
#define FRAME_OPCODE_PONG     0xa

#define FRAME_PAYLOAD_SIZE_7E 0x7e
#define FRAME_PAYLOAD_SIZE_7F 0x7f

#define FRAME_PAYLOAD_LENGTH_MAX_7E 0xffff

#define FRAME_PAYLOAD_LENGTH_LENGTH_7E 2
#define FRAME_PAYLOAD_LENGTH_LENGTH_7F 8

typedef struct client_info_t
{
  int socket_fd;
  char ip[IP_SIZE];
  int port;
  bool in_header;
} client_info_t;

typedef void callback_connect_t(client_info_t*, void*);
typedef void callback_disconnect_t(client_info_t*, void*);
typedef bool callback_recv_t(client_info_t*, void*, uint8_t*, int);
typedef bool callback_poll_t(client_info_t*, void*);
typedef bool callback_periodic_t(client_info_t*, void*);

#endif
