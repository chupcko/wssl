#ifndef _FRAME_H_
#define _FRAME_H_

_INCLUDE_BEGIN_

typedef uint64_t wssl_frame_size_t;

#define WSSL_FRAME_MASKING_KEY_SIZE 4
typedef struct wssl_frame_t
{
  bool              fin;
  wssl_octet_t      opcode;
  bool              masked;
  wssl_octet_t      payload_size;
  wssl_octet_t      masking_key[WSSL_FRAME_MASKING_KEY_SIZE];
  wssl_octet_t*     payload;
  wssl_frame_size_t payload_length;
} wssl_frame_t;

_INCLUDE_END_

#define FRAME_OPCODE_CONTINUE 0x0
#define FRAME_OPCODE_TEXT     0x1
#define FRAME_OPCODE_BINARY   0x2
#define FRAME_OPCODE_CLOSE    0x8
#define FRAME_OPCODE_PING     0x9
#define FRAME_OPCODE_PONG     0xa

#define FRAME_PAYLOAD_SIZE_MEDIUM 0x7e
#define FRAME_PAYLOAD_SIZE_LONG   0x7f

#define FRAME_PAYLOAD_LENGTH_SHORT_MAX  0x7e
#define FRAME_PAYLOAD_LENGTH_MEDIUM_MAX 0xffff

#define FRAME_PAYLOAD_LENGTH_MEDIUM_LENGTH 2
#define FRAME_PAYLOAD_LENGTH_LONG_LENGTH   8

static inline void wssl_frame_init
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  frame->payload = WSSL_NULL;
  frame->payload_length = 0;
}

static inline void frame_mask_unmask
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  if
  (
    frame->masked &&
    frame->payload != WSSL_NULL
  )
  {
    wssl_frame_size_t index;
    for(index = 0; index < frame->payload_length; index++)
      frame->payload[index] ^= frame->masking_key[index%WSSL_FRAME_MASKING_KEY_SIZE];
  }
}

#endif
