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
  wssl_octet_t      length;
  wssl_octet_t      masking_key[WSSL_FRAME_MASKING_KEY_SIZE];
  wssl_octet_t*     payload;
  wssl_frame_size_t payload_size;
  wssl_size_t       count;
} wssl_frame_t;

_INCLUDE_END_

#define FRAME_OPCODE_CONTINUE 0x0
#define FRAME_OPCODE_TEXT     0x1
#define FRAME_OPCODE_BINARY   0x2
#define FRAME_OPCODE_CLOSE    0x8
#define FRAME_OPCODE_PING     0x9
#define FRAME_OPCODE_PONG     0xa

#define FRAME_LENGTH_LONG          0x7f
#define FRAME_LENGTH_MEDIUM        0x7e
#define FRAME_LENGTH_LONG_LENGTH   8
#define FRAME_LENGTH_MEDIUM_LENGTH 2
#define FRAME_PAYLOAD_SIZE_MEDIUM  0xffff
#define FRAME_PAYLOAD_SIZE_SHORT   0x7e

static inline void wssl_frame_init
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  frame->payload = WSSL_NULL;
  frame->payload_size = 0;
  frame->count = 0;
}

static inline bool wssl_frame_is_allocated
(
  _WSSL_IN_ const wssl_frame_t* frame
)
{
  return frame->count != 0;
}

static inline bool wssl_frame_is_not_allocated
(
  _WSSL_IN_ const wssl_frame_t* frame
)
{
  return frame->count == 0;
}

static inline void wssl_frame_free
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  free((void*)frame->payload);
  frame->payload = WSSL_NULL;
  frame->payload_size = 0;
  frame->count = 0;
}

static inline void wssl_frame_mask_unmask
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
    for(index = 0; index < frame->payload_size; index++)
      frame->payload[index] ^= frame->masking_key[index%WSSL_FRAME_MASKING_KEY_SIZE];
  }
}

#endif
