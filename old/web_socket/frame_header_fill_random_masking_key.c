#include "main.h"

void frame_header_fill_random_masking_key(frame_t* frame)
{
  int i;

  frame->masked = true;
  for(i = 0; i < FRAME_MASKING_KEY_SIZE; i++)
    frame->masking_key[i] = (uint8_t)(rand()&0xff);
}
