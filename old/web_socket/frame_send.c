#include "main.h"

void frame_send(client_info_t* client_info, uint8_t opcode, char* message, int message_length)
{
  frame_t frame_out;
  int out_buffer_length;

  frame_out.fin = true;
  frame_out.opcode = opcode;
  frame_out.masked = false;
  frame_out.payload_length = message_length;
  frame_out.payload = message;
  out_buffer_length = put_frame_header(&frame_out, Out_buffer, BUFFER_SIZE);
  if(out_buffer_length < 0)
    error("Out buffer is full");
  client_send(client_info->socket_fd, Out_buffer, out_buffer_length);
}
