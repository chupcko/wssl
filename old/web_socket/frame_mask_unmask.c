#include "main.h"

void frame_mask_unmask(frame_t* frame)
{
  if(frame->masked && frame->payload != NULL)
  {
    int i;

    for(i = 0; i < frame->payload_length; i++)
      frame->payload[i] ^= frame->masking_key[i%FRAME_MASKING_KEY_SIZE];
  }
}
