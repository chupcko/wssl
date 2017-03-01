#include "main.h"

bool frame_execute(client_info_t* client_info, frame_t* frame_in, uint8_t* frame_in_buffer, int frame_in_buffer_length)
{
  frame_t frame_out;
  int out_buffer_length;

  switch(frame_in->opcode)
  {
    case FRAME_OPCODE_TEXT:
    case FRAME_OPCODE_BINARY:
      if
      (
        Callback_recv != NULL &&
        (*Callback_recv)(client_info, Callback_extra_data, frame_in_buffer, frame_in_buffer_length) != true
      )
        return false;
      break;
    case FRAME_OPCODE_CLOSE:
      frame_send(client_info, FRAME_OPCODE_CLOSE, NULL, 0);
      return false;
      break;
    case FRAME_OPCODE_PING:
      frame_send(client_info, FRAME_OPCODE_PONG, frame_in_buffer, frame_in_buffer_length);
      break;
    case FRAME_OPCODE_PONG:
      debug("Got PONG");
      break;
  }
  return true;
}
