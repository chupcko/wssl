#include "main.h"

_FUNCTION_
wssl_result_t wssl_frame_put
(
  _WSSL_IN_  const wssl_frame_t* frame,
  _WSSL_OUT_       wssl_octet_t* data,
  _WSSL_IN_  const wssl_size_t   data_size,
  _WSSL_OUT_       wssl_size_t*  data_length
)
{
  wssl_frame_size_t payload_size;
  wssl_size_t i;

  *data_length = 0;

  if(*data_length >= data_size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
  data[*data_length] = frame->opcode&0x0f;
  if(frame->fin)
    data[*data_length] |= 0x80;
  ++*data_length;

  if(*data_length >= data_size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
  data[*data_length] =  frame->length;
  if(frame->masked)
    data[*data_length] |= 0x80;
  ++*data_length;

  switch(frame->length)
  {
    case FRAME_LENGTH_LONG_CODE:
      if(*data_length+FRAME_LENGTH_LONG_LENGTH > data_size)
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
      payload_size = frame->payload_size;
      for(i = 0; i < FRAME_LENGTH_LONG_LENGTH; i++)
      {
        data[*data_length+FRAME_LENGTH_LONG_LENGTH-1-i] = (wssl_octet_t)(payload_size&0xff);
        payload_size >>= WSSL_OCTET_SIZE_IN_BITS;
      }
      *data_length += FRAME_LENGTH_LONG_LENGTH;
      break;
    case FRAME_LENGTH_MEDIUM_CODE:
      if(*data_length+FRAME_LENGTH_MEDIUM_LENGTH > data_size)
        return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
      payload_size = frame->payload_size;
      for(i = 0; i < FRAME_LENGTH_MEDIUM_LENGTH; i++)
      {
        data[*data_length+FRAME_LENGTH_MEDIUM_LENGTH-1-i] = (wssl_octet_t)(payload_size&0xff);
        payload_size >>= WSSL_OCTET_SIZE_IN_BITS;
      }
      *data_length += FRAME_LENGTH_MEDIUM_LENGTH;
      break;
  }

  if(frame->masked)
  {
    if(*data_length+WSSL_FRAME_MASKING_KEY_SIZE > data_size)
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
    for(i = 0; i < WSSL_FRAME_MASKING_KEY_SIZE; i++)
      data[*data_length+i] = frame->masking_key[i];
    *data_length += WSSL_FRAME_MASKING_KEY_SIZE;
  }

  if(frame->payload_size > 0)
  {
    if(frame->payload == WSSL_NULL)
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_CONSISTENCY, "frame_put", 0);
    if(*data_length+(wssl_size_t)frame->payload_size > data_size)
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "frame_put", 0);
    if(frame->masked)
      for(i = 0; i < frame->payload_size; i++)
        data[*data_length+i] = frame->payload[i]^frame->masking_key[i%WSSL_FRAME_MASKING_KEY_SIZE];
    else
      memcpy((void*)&data[*data_length], (void*)frame->payload, (size_t)frame->payload_size);
    *data_length += (wssl_size_t)frame->payload_size;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
