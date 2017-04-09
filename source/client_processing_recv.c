#include "main.h"

static inline
wssl_ssize_t wssl_client_processing_recv_find_word_until_space
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case ' ':
      case '\t':
        return size;
        break;
      case '\r':
      case '\n':
        return -1;
        break;
    }
  return data_size;
}

static inline
wssl_size_t wssl_client_processing_recv_find_word_until_crlf
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_size_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case '\r':
      case '\n':
        return size;
        break;
    }
  return data_size;
}

static inline
wssl_ssize_t wssl_client_processing_recv_find_word_until_colon
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case ':':
        return size;
        break;
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        return -1;
        break;
    }
  return data_size;
}

static inline
wssl_ssize_t wssl_client_processing_recv_eat_spaces
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case ' ':
      case '\t':
        break;
      case '\r':
      case '\n':
        return -1;
        break;
      default:
        return size;
        break;
    }
  return data_size;
}

static inline
wssl_size_t wssl_client_processing_recv_count_spaces
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_size_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case ' ':
      case '\t':
        break;
      default:
        return size;
        break;
    }
  return data_size;
}

static inline
wssl_ssize_t wssl_client_processing_recv_eat_crlf
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size;
  for(size = 0; size < data_size; size++)
    switch(data[size])
    {
      case '\r':
        break;
      case '\n':
        return size+1;
        break;
      default:
        return -1;
        break;
    }
  return 0;
}

_FUNCTION_
wssl_result_t wssl_client_processing_recv
(
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_           wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size,
  _WSSL_OUT_          wssl_size_t*   processed
)
{
  wssl_ssize_t size;

  *processed = 0;

  switch(client->state)
  {
    case WSSL_CLIENT_STATE_WAIT_METHOD:
      size = wssl_client_processing_recv_find_word_until_space((char*)data, data_size);
      if(size <= 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_METHOD);
      if(size < data_size)
      {
        TRY_CALL(wssl_header_insert_method(&client->header, (char*)data, size));
        client->state = WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR:
      size = wssl_client_processing_recv_eat_spaces((char*)data, data_size);
      if(size <= 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_URI);
      if(size < data_size)
      {
        client->state = WSSL_CLIENT_STATE_WAIT_URI;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_URI:
      size = wssl_client_processing_recv_find_word_until_space((char*)data, data_size);
      if(size <= 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_URI);
      if(size < data_size)
      {
        TRY_CALL(wssl_header_insert_uri(&client->header, (char*)data, size));
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR:
      size = wssl_client_processing_recv_eat_spaces((char*)data, data_size);
      if(size <= 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_VERSION);
      if(size < data_size)
      {
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION:
      size = wssl_client_processing_recv_find_word_until_crlf((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        TRY_CALL(wssl_header_insert_version(&client->header, (char*)data, size));
        client->state = WSSL_CLIENT_STATE_WAIT_CRLF;
        *processed = size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_CRLF:
      size = wssl_client_processing_recv_eat_crlf((char*)data, data_size);
      if(size < 0)
        MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_CRLF);
      client->state = WSSL_CLIENT_STATE_WAIT_FIELD_KEY;
      *processed = (wssl_size_t)size;
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_KEY:
      size = wssl_client_processing_recv_eat_crlf((char*)data, data_size);
      if(size > 0)
      {
        client->state = WSSL_CLIENT_STATE_PROCESSING_HEADER;
        TRY_CALL(wssl_client_processing_header(client));
        PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
        *processed = (wssl_size_t)size;
      }
      else
      {
        size = wssl_client_processing_recv_find_word_until_colon((char*)data, data_size);
        if(size <= 0)
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_MISSING_FIELD_KEY);
        if(size < data_size)
        {
          TRY_CALL(wssl_header_add_field(&client->header, (char*)data, size));
          client->state = WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR;
          *processed = (wssl_size_t)size;
        }
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR:
      size = wssl_client_processing_recv_count_spaces((char*)&data[1], data_size-1);
      if(size >= 0 && size < data_size-1)
      {
        client->state = WSSL_CLIENT_STATE_WAIT_FIELD_VALUE;
        *processed = (wssl_size_t)size+1;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE:
      size = wssl_client_processing_recv_find_word_until_crlf((char*)data, data_size);
      if(size >= 0 && size < data_size)
      {
        TRY_CALL(wssl_header_insert_value_at_last_field(&client->header, (char*)data, size));
        client->state = WSSL_CLIENT_STATE_WAIT_CRLF;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FRAME:
      size = wssl_frame_get(&client->frame, data, data_size);
      if(size > 0)
      {
        if(client->frame.opcode == FRAME_OPCODE_CONTINUE)
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_FRAME_OPCODE);

        TRY_CALL(wssl_frame_allocate(&client->frame));/*# pomeri samo ako nema */
        if(client->frame.fin)
        {
          TRY_CALL(wssl_client_processing_frame(client));
          PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
          wssl_frame_free(&client->frame);
        }
        else
          client->state = WSSL_CLIENT_STATE_WAIT_FIN_FRAME;
        *processed = (wssl_size_t)size;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIN_FRAME:
    {
      wssl_frame_t frame;
      wssl_frame_init(&frame);
      size = wssl_frame_get(&frame, data, data_size);
      if(size > 0)
      {
        if(frame.opcode != FRAME_OPCODE_CONTINUE)
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_FRAME_OPCODE);
        if(client->number_of_received_multi_frames > client->wssl->max_number_of_received_multi_frames)
          MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_TOO_MUCH_RECEIVED_MULTI_FRAMES);

        TRY_CALL(wssl_frame_reallocate(&client->frame, &frame));
        if(frame.fin)
        {
          TRY_CALL(wssl_client_processing_frame(client));
          PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
          wssl_frame_free(&client->frame);
          client->state = WSSL_CLIENT_STATE_WAIT_FRAME;
        }
        client->number_of_received_multi_frames++;
        *processed = (wssl_size_t)size;
      }
      break;
    }
  }

  PASS;
}
