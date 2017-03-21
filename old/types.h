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
