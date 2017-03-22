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
} wssl_frame_t;

_INCLUDE_END_

#define FRAME_OPCODE_CONTINUE 0x0
#define FRAME_OPCODE_TEXT     0x1
#define FRAME_OPCODE_BINARY   0x2
#define FRAME_OPCODE_CLOSE    0x8
#define FRAME_OPCODE_PING     0x9
#define FRAME_OPCODE_PONG     0xa

#define FRAME_LENGTH_LONG_CODE     0x7f
#define FRAME_LENGTH_MEDIUM_CODE   0x7e
#define FRAME_LENGTH_LONG_LENGTH   8
#define FRAME_LENGTH_MEDIUM_LENGTH 2
#define FRAME_PAYLOAD_SIZE_MEDIUM  0xffff
#define FRAME_PAYLOAD_SIZE_SHORT   0x7d

static inline void wssl_frame_init
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  frame->fin          = true;
  frame->opcode       = FRAME_OPCODE_CONTINUE;
  frame->masked       = false;
  frame->length       = 0;
  frame->payload      = WSSL_NULL;
  frame->payload_size = 0;
}

static inline wssl_result_t wssl_frame_allocate
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  wssl_octet_t* payload = (wssl_octet_t*)malloc((size_t)frame->payload_size+1);
  if(payload == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "frame", 0);
  memcpy((void*)payload, (void*)frame->payload, (size_t)frame->payload_size);
  frame->payload = payload;
  frame->payload[frame->payload_size] = '\0';
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline wssl_result_t wssl_frame_reallocate
(
  _WSSL_MODIFY_       wssl_frame_t* frame_destination,
  _WSSL_IN_     const wssl_frame_t* frame_source
)
{
  wssl_octet_t* payload = (wssl_octet_t*)realloc
  (
    (void*)frame_destination->payload,
    (size_t)(frame_destination->payload_size+frame_source->payload_size)+1
  );
  if(payload == NULL)
  {
    free((void*)frame_destination->payload);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "frame", 0);
  }
  memcpy((void*)&payload[frame_destination->payload_size], (void*)frame_source->payload, (size_t)frame_source->payload_size);
  frame_destination->payload = payload;
  frame_destination->payload_size += frame_source->payload_size;
  frame_destination->payload[frame_destination->payload_size] = '\0';
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline bool wssl_frame_is_allocated
(
  _WSSL_IN_ const wssl_frame_t* frame
)
{
  return frame->payload != WSSL_NULL;
}

static inline bool wssl_frame_is_not_allocated
(
  _WSSL_IN_ const wssl_frame_t* frame
)
{
  return frame->payload == WSSL_NULL;
}

static inline void wssl_frame_free
(
  _WSSL_MODIFY_ wssl_frame_t* frame
)
{
  free((void*)frame->payload);
  frame->payload = WSSL_NULL;
  frame->payload_size = 0;
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
    wssl_frame_size_t i;
    for(i = 0; i < frame->payload_size; i++)
      frame->payload[i] ^= frame->masking_key[i%WSSL_FRAME_MASKING_KEY_SIZE];
  }
}

#endif
