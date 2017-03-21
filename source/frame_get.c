#include "main.h"

_FUNCTION_
wssl_size_t wssl_frame_get
(
  _WSSL_MODIFY_       wssl_frame_t* frame,
  _WSSL_IN_           wssl_octet_t* data,
  _WSSL_IN_     const wssl_size_t   data_size
)
{
  wssl_size_t i;
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
      for(i = 0; i < FRAME_LENGTH_LONG_LENGTH; i++)
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
      for(i = 0; i < FRAME_LENGTH_MEDIUM_LENGTH; i++)
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

  if(frame->masked)
  {
    if(processed+WSSL_FRAME_MASKING_KEY_SIZE >= data_size)
      return 0;
    for(i = 0; i < WSSL_FRAME_MASKING_KEY_SIZE; i++)
    {
      frame->masking_key[i] = data[processed];
      processed++;
    }
  }

  if(processed+(wssl_size_t)frame->payload_size > data_size)
    return 0;
  frame->payload = &data[processed];
  processed += (wssl_size_t)frame->payload_size;

  wssl_frame_mask_unmask(frame);

  return processed;
}
