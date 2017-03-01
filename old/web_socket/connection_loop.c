#include "main.h"

char recv_buffer[BUFFER_SIZE];
int recv_buffer_length = 0;

char frame_buffer[BUFFER_SIZE];
int frame_buffer_length = 0;
frame_t frame_first;

bool connection_ws(client_info_t* client_info, header_data_t* header_data)
{
  int recv_size;
  int processed_size;
  int recv_buffer_begin;
  frame_t frame_local;
  frame_t* frame = &frame_first;

  if(BUFFER_SIZE-1-recv_buffer_length <= 0)
    error("Recv buffer is full");

  recv_size = recv(client_info->socket_fd, &recv_buffer[recv_buffer_length], BUFFER_SIZE-1-recv_buffer_length, 0);
  if(recv_size < 0)
  {
    if(errno == ECONNRESET || errno == EPIPE)
      return true;
    error("Cannot recv(%d %s)", errno, strerror(errno));
  }
  if(recv_size == 0)
    return false;
  recv_buffer_length += recv_size;
  recv_buffer[recv_buffer_length]='\0';

  recv_buffer_begin = 0;
  while(recv_buffer_begin < recv_buffer_length)
  {
    if(client_info->in_header)
    {
      processed_size = header_parser(&recv_buffer[recv_buffer_begin], recv_buffer_length-recv_buffer_begin, header_data);
      if(processed_size >= 0 && header_data[HEADER_INDEX_END].value_length != -1)
      {
        client_info->in_header = false;
        if(header_response(client_info->socket_fd, header_data) != true)
          return false;
      }
    }
    else
    {
      processed_size = get_frame_header(&recv_buffer[recv_buffer_begin], recv_buffer_length-recv_buffer_begin, frame);
      if(processed_size >= 0)
      {
        frame_mask_unmask(frame);
        if(frame_buffer_length+frame->payload_length+1 >= BUFFER_SIZE)
          error("Frame buffer is full");
        memcpy(&frame_buffer[frame_buffer_length], frame->payload, frame->payload_length);
        frame_buffer_length += frame->payload_length;
        frame_buffer[frame_buffer_length] = '\0';
        if(frame->fin)
        {
          if(frame_execute(client_info, &frame_first, frame_buffer, frame_buffer_length) != true)
            return false;
          frame_buffer_length = 0;
          frame = &frame_first;
        }
        else
          frame = &frame_local;
      }
    }
    if(processed_size < 0)
      break;
    recv_buffer_begin += processed_size;
  }
  if(recv_buffer_begin < recv_buffer_length)
  {
    memmove(recv_buffer, &recv_buffer[recv_buffer_begin], recv_buffer_length-recv_buffer_begin);
    recv_buffer_length = recv_buffer_length-recv_buffer_begin;
    recv_buffer[recv_buffer_length]='\0';
  }
  else
    recv_buffer_length = 0;

  return true;
}

void connection_loop(client_info_t* client_info)
{
  header_data_t header_data[HEADER_INDEX_SIZE];
  struct pollfd poll_data[2];
  int poll_data_size;
  int i;

  client_info->in_header = true;
  for(i = 0; i < HEADER_INDEX_SIZE; i++)
  {
    header_data[i].value = NULL;
    header_data[i].value_length = -1;
  };

  poll_data_size = 0;
  poll_data[poll_data_size].fd = client_info->socket_fd;
  poll_data[poll_data_size].events = POLLIN;
  poll_data_size++;
  if(Callback_poll != NULL)
  {
    poll_data[poll_data_size].fd = Callback_poll_fd;
    poll_data[poll_data_size].events = POLLIN;
    poll_data_size++;
  }

  loop
  {
    poll_data[0].revents = 0;
    poll_data[1].revents = 0;
    if(poll(poll_data, poll_data_size, POLL_TIMEOUT_MS) < 0)
      error("Cannot poll(%d %s)", errno, strerror(errno));
    if
    (
      poll_data[0].revents != 0 &&
      connection_ws(client_info, header_data) != true
    )
      return;
    if
    (
      poll_data[1].revents != 0 &&
      Callback_poll != NULL &&
      (*Callback_poll)(client_info, Callback_extra_data) != true
    )
      return;
    if
    (
      Callback_periodic != NULL &&
      (*Callback_periodic)(client_info, Callback_extra_data) != true
    )
      return;
  }
}
