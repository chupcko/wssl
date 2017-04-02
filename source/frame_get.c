#include "main.h"

_FUNCTION_
wssl_size_t wssl_frame_get
(
  _WSSL_MODIFY_       wssl_frame_t* frame,
  _WSSL_MODIFY_       wssl_octet_t* data,
  _WSSL_IN_     const wssl_size_t   data_size
)
{
  wssl_size_t i;
  wssl_size_t data_length = 0;

  if(data_length+1 > data_size)
    return 0;
  frame->fin = (data[data_length]&0x80) != 0;
  frame->opcode = data[data_length]&0x0f;
  data_length++;

  if(data_length+1 > data_size)
    return 0;
  frame->masked = (data[data_length]&0x80) != 0;
  frame->length = data[data_length]&0x7f;
  data_length++;

  switch(frame->length)
  {
    case FRAME_LENGTH_LONG_CODE:
      if(data_length+FRAME_LENGTH_LONG_LENGTH > data_size)
        return 0;
      frame->payload_size = 0;
      for(i = 0; i < FRAME_LENGTH_LONG_LENGTH; i++)
      {
        frame->payload_size <<= OCTET_SIZE_IN_BITS;
        frame->payload_size |= (wssl_frame_size_t)data[data_length+i];
      }
      data_length += FRAME_LENGTH_LONG_LENGTH;
      break;
    case FRAME_LENGTH_MEDIUM_CODE:
      if(data_length+FRAME_LENGTH_MEDIUM_LENGTH > data_size)
        return 0;
      frame->payload_size = 0;
      for(i = 0; i < FRAME_LENGTH_MEDIUM_LENGTH; i++)
      {
        frame->payload_size <<= OCTET_SIZE_IN_BITS;
        frame->payload_size |= (wssl_frame_size_t)data[data_length+i];
      }
      data_length += FRAME_LENGTH_MEDIUM_LENGTH;
      break;
    default:
      frame->payload_size = (wssl_frame_size_t)frame->length;
      break;
  }

  if(frame->masked)
  {
    if(data_length+WSSL_FRAME_MASKING_KEY_SIZE > data_size)
      return 0;
    for(i = 0; i < WSSL_FRAME_MASKING_KEY_SIZE; i++)
      frame->masking_key[i] = data[data_length+i];
    data_length += WSSL_FRAME_MASKING_KEY_SIZE;
  }

  if(data_length+(wssl_size_t)frame->payload_size > data_size)
    return 0;
  frame->payload = &data[data_length];
  data_length += (wssl_size_t)frame->payload_size;

  if(frame->masked)
    for(i = 0; i < frame->payload_size; i++)
      frame->payload[i] ^= frame->masking_key[i%WSSL_FRAME_MASKING_KEY_SIZE];

  return data_length;
}
