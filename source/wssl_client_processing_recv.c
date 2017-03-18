#include "main.h"

static inline wssl_ssize_t wssl_client_processing_recv_find_word_until_space
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

static inline wssl_ssize_t wssl_client_processing_recv_find_word_until_crlf
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
      case '\n':
        return size;
        break;
    }
  return data_size;
}

static inline wssl_ssize_t wssl_client_processing_recv_find_word_until_colon
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

static inline wssl_ssize_t wssl_client_processing_recv_eat_spaces
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

static inline wssl_ssize_t wssl_client_processing_recv_count_spaces
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
      default:
        return size;
        break;
    }
  return data_size;
}

static inline wssl_ssize_t wssl_client_processing_recv_eat_crlf
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
  _WSSL_OUT_          bool*          client_deleted,
  _WSSL_IN_     const wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size,
  _WSSL_OUT_          wssl_size_t*   processed
)
{
  wssl_ssize_t size;

  *client_deleted = false;
  *processed = 0;

  switch(client->state)
  {
    case WSSL_CLIENT_STATE_WAIT_METHOD:
      size = wssl_client_processing_recv_find_word_until_space((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_method(&client->header, (char*)data, size));
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR;
      }
      else if(size <= 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        *client_deleted = true;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR:
      size = wssl_client_processing_recv_eat_spaces((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_URI;
      }
      else if(size <= 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        *client_deleted = true;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_URI:
      size = wssl_client_processing_recv_find_word_until_space((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_uri(&client->header, (char*)data, size));
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR;
      }
      else if(size <= 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        *client_deleted = true;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR:
      size = wssl_client_processing_recv_eat_spaces((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION;
      }
      else if(size <= 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        *client_deleted = true;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION:
      size = wssl_client_processing_recv_find_word_until_crlf((char*)data, data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_version(&client->header, (char*)data, size));
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_CRLF;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_CRLF:
      size = wssl_client_processing_recv_eat_crlf((char*)data, data_size);
      if(size > 0)
      {
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_FIELD_KEY;
      }
      else if(size < 0)
      {
        WSSL_CALL(wssl_client_delete(client));
        *client_deleted = true;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_KEY:
      size = wssl_client_processing_recv_eat_crlf((char*)data, data_size);
      if(size > 0)
      {
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_FRAME;
        WSSL_CALL(wssl_client_processing_header(client, client_deleted));
      }
      else
      {
        size = wssl_client_processing_recv_find_word_until_colon((char*)data, data_size);
        if(size > 0 && size < data_size)
        {
          WSSL_CALL(wssl_header_add_field(&client->header, (char*)data, size));
          *processed = (wssl_size_t)size;
          client->state = WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR;
        }
        else if(size <= 0)
        {
          WSSL_CALL(wssl_client_delete(client));
          *client_deleted = true;
        }
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR:
      size = wssl_client_processing_recv_count_spaces((char*)&data[1], data_size-1);
      if(size >= 0 && size < data_size-1)
      {
        *processed = (wssl_size_t)size+1;
        client->state = WSSL_CLIENT_STATE_WAIT_FIELD_VALUE;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE:
      size = wssl_client_processing_recv_find_word_until_crlf((char*)data, data_size);
      if(size >= 0 && size < data_size)
      {
        wssl_header_field_t* last_headed_field = wssl_header_get_last_field(&client->header);
        if(last_headed_field == WSSL_NULL)
          return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_CONSISTENCY, "header field", 0);
        WSSL_CALL(wssl_header_field_insert_value(last_headed_field, (char*)data, size));
        *processed = (wssl_size_t)size;
        client->state = WSSL_CLIENT_STATE_WAIT_CRLF;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_FRAME:
      /*#*/
      break;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
