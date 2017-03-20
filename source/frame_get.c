#include "main.h"

_FUNCTION_
wssl_size_t wssl_frame_get
(
  _WSSL_IN_     const wssl_t*       wssl,
  _WSSL_MODIFY_       wssl_frame_t* frame,
  _WSSL_IN_           wssl_octet_t* data,
  _WSSL_IN_     const wssl_size_t   data_size
)
{
  wssl_size_t index;
  wssl_size_t processed = 0;

  if(processed >= data_size)
    return 0;
  frame->fin = (data[processed]&0x80) != 0;
  frame->opcode = data[processed]&0x0f;
  processed++;

  if(processed >= data_size)
    return 0;
  frame->masked = (data[processed]&0x80) != 0;
  frame->length = data[processed]&0x7f;
  processed++;

  switch(frame->length)
  {
    case FRAME_LENGTH_LONG:
      if(processed+FRAME_LENGTH_LONG_LENGTH >= data_size)
        return 0;
      frame->payload_size = 0;
      for(index = 0; index < FRAME_LENGTH_LONG_LENGTH; index++)
      {
        frame->payload_size <<= WSSL_OCTET_SIZE_IN_BITS;
        frame->payload_size |= (wssl_frame_size_t)data[processed];
        processed++;
      }
      break;
    case FRAME_LENGTH_MEDIUM:
      if(processed+FRAME_LENGTH_MEDIUM_LENGTH >= data_size)
        return 0;
      frame->payload_size = 0;
      for(index = 0; index < FRAME_LENGTH_MEDIUM_LENGTH; index++)
      {
        frame->payload_size <<= WSSL_OCTET_SIZE_IN_BITS;
        frame->payload_size |= (wssl_frame_size_t)data[processed];
        processed++;
      }
      break;
    default:
      frame->payload_size = (wssl_frame_size_t)frame->length;
      break;
  }

/*# proveri da li je ulaz prevelik */

  if(frame->masked)
  {
    if(processed+WSSL_FRAME_MASKING_KEY_SIZE >= data_size)
      return 0;
    for(index = 0; index < WSSL_FRAME_MASKING_KEY_SIZE; index++)
    {
      frame->masking_key[index] = data[processed];
      processed++;
    }
  }

  if(processed+(wssl_size_t)frame->payload_size > data_size)
    return 0;
  frame->payload = &data[processed];
  processed += (wssl_size_t)frame->payload_size;

  return processed;
}
