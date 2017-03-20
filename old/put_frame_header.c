#include "main.h"

int put_frame_header(frame_t* frame, uint8_t* buffer, int buffer_size)
{
  int i;
  uint64_t payload_length;

  frame->payload_offset = 0;

  if(frame->payload_offset > buffer_size)
    return -1;
  buffer[frame->payload_offset] = frame->opcode&0x0f;
  if(frame->fin)
    buffer[frame->payload_offset] |= 0x80;
  frame->payload_offset++;

  if(frame->payload_offset >= buffer_size)
    return -1;
  if(frame->payload_length < FRAME_PAYLOAD_SIZE_7E)
  {
    frame->payload_size = (uint8_t)frame->payload_length;
    buffer[frame->payload_offset] = (uint8_t)frame->payload_length;
  }
  else if(frame->payload_length <= FRAME_PAYLOAD_LENGTH_MAX_7E)
  {
    frame->payload_size = FRAME_PAYLOAD_LENGTH_LENGTH_7E;
    buffer[frame->payload_offset] = FRAME_PAYLOAD_LENGTH_LENGTH_7E;
  }
  else
  {
    frame->payload_size = FRAME_PAYLOAD_LENGTH_LENGTH_7F;
    buffer[frame->payload_offset] = FRAME_PAYLOAD_LENGTH_LENGTH_7F;
  }
  if(frame->masked)
    buffer[frame->payload_offset] |= 0x80;
  frame->payload_offset++;

  if(frame->payload_size == 0x7e)
  {
    if(frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7E > buffer_size)
      return -1;
    payload_length = frame->payload_length;
    for(i = 0; i < FRAME_PAYLOAD_LENGTH_LENGTH_7E; i++)
    {
      buffer[frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7E-1-i] = (uint8_t)(payload_length&0xff);
      payload_length >>= BITS_IN_BYTE;
    }
    frame->payload_offset += FRAME_PAYLOAD_LENGTH_LENGTH_7E;
  }
  else if(frame->payload_size == 0x7f)
  {
    if(frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7F > buffer_size)
      return -1;
    payload_length = frame->payload_length;
    for(i = 0; i < FRAME_PAYLOAD_LENGTH_LENGTH_7F; i++)
    {
      buffer[frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7F-1-i] = (uint8_t)(payload_length&0xff);
      payload_length >>= BITS_IN_BYTE;
    }
    frame->payload_offset += FRAME_PAYLOAD_LENGTH_LENGTH_7F;
  }

  if(frame->masked)
  {
    if(frame->payload_offset+FRAME_MASKING_KEY_SIZE > buffer_size)
      return -1;
    for(i = 0; i < FRAME_MASKING_KEY_SIZE; i++)
      buffer[frame->payload_offset+i] = frame->masking_key[i];
    frame->payload_offset += FRAME_MASKING_KEY_SIZE;
  }

  if(frame->payload_offset+frame->payload_length > buffer_size)
    return -1;
  if(frame->payload_length > 0 && frame->payload != NULL)
  {
    memcpy(&buffer[frame->payload_offset], frame->payload, frame->payload_length);
    frame->payload = &buffer[frame->payload_offset];
  }
  frame_mask_unmask(frame);

  return frame->payload_offset+frame->payload_length;
}
