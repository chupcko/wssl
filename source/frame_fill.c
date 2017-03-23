#include "main.h"

_FUNCTION_
void wssl_frame_fill
(
  _WSSL_MODIFY_       wssl_t*       wssl,
  _WSSL_MODIFY_       wssl_frame_t* frame,
  _WSSL_IN_     const wssl_octet_t  opcode,
  _WSSL_IN_     const bool          masked,
  _WSSL_IN_           wssl_octet_t* payload,
  _WSSL_IN_     const wssl_size_t   payload_size
)
{
  frame->fin          = true;
  frame->opcode       = opcode;
  frame->masked       = masked;
  if(payload_size <= FRAME_PAYLOAD_SIZE_SHORT)
    frame->length = (wssl_octet_t)payload_size;
  else if(payload_size <= FRAME_PAYLOAD_SIZE_MEDIUM)
    frame->length = FRAME_LENGTH_MEDIUM_CODE;
  else
    frame->length = FRAME_LENGTH_LONG_CODE;
  frame->payload      = payload;
  frame->payload_size = payload_size;
  if(masked)
  {
    wssl_size_t i;
    for(i = 0; i < WSSL_FRAME_MASKING_KEY_SIZE; i++)
      frame->masking_key[i] = (wssl_octet_t)(rand_r(&wssl->random_seed)&0xff);
  }
}
