#include "main.h"

int get_frame_header(uint8_t* buffer, int buffer_size, frame_t* frame)
{
  int i;

  frame->payload_offset = 0;

  if(frame->payload_offset > buffer_size)
    return -1;
  frame->fin = (buffer[frame->payload_offset]&0x80) != 0;
  frame->opcode = buffer[frame->payload_offset]&0x0f;
  frame->payload_offset++;

  if(frame->payload_offset > buffer_size)
    return -1;
  frame->masked = (buffer[frame->payload_offset]&0x80) != 0;
  frame->payload_size = buffer[frame->payload_offset]&0x7f;
  frame->payload_offset++;

  if(frame->payload_size == FRAME_PAYLOAD_SIZE_7E)
  {
    if(frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7E > buffer_size)
      return -1;
    frame->payload_length = 0;
    for(i = 0; i < FRAME_PAYLOAD_LENGTH_LENGTH_7E; i++)
    {
      frame->payload_length <<= BITS_IN_BYTE;
      frame->payload_length |= (uint64_t)buffer[frame->payload_offset+i];
    }
    frame->payload_offset += FRAME_PAYLOAD_LENGTH_LENGTH_7E;
  }
  else if(frame->payload_size == FRAME_PAYLOAD_SIZE_7F)
  {
    if(frame->payload_offset+FRAME_PAYLOAD_LENGTH_LENGTH_7F > buffer_size)
      return -1;
    frame->payload_length = 0;
    for(i = 0; i < FRAME_PAYLOAD_LENGTH_LENGTH_7F; i++)
    {
      frame->payload_length <<= BITS_IN_BYTE;
      frame->payload_length |= (uint64_t)buffer[frame->payload_offset+i];
    }
    frame->payload_offset += FRAME_PAYLOAD_LENGTH_LENGTH_7F;
  }
  else
    frame->payload_length = (uint64_t)frame->payload_size;

  if(frame->masked)
  {
    if(frame->payload_offset+FRAME_MASKING_KEY_SIZE > buffer_size)
      return -1;
    for(i = 0; i < FRAME_MASKING_KEY_SIZE; i++)
      frame->masking_key[i] = buffer[frame->payload_offset+i];
    frame->payload_offset += FRAME_MASKING_KEY_SIZE;
  }

  if(frame->payload_offset+frame->payload_length > buffer_size)
    return -1;
  frame->payload = &buffer[frame->payload_offset];

  return frame->payload_offset+frame->payload_length;
}
