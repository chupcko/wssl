#include "main.h"

static inline wssl_ssize_t wssl_client_processing_find_word
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size;

  for(size = 0; size < data_size; size++)
    if(isspace(data[size]) != 0)
      return size;
  return data_size;
}

static inline wssl_ssize_t wssl_client_processing_count_spaces
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_ssize_t size = 0;

  for(size = 0; size < data_size; size++)
    if(data[size] != ' ')
      if(data[size] == '\r' || data[size] == '\n')
        return -1;
      else
        return size;
  return data_size;
}

_FUNCTION_
wssl_result_t wssl_client_processing
(
  _WSSL_MODIFY_       wssl_t*        wssl,
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_size_t    data_begin,
  _WSSL_IN_     const wssl_size_t    data_size,
  _WSSL_OUT_          wssl_ssize_t*  processed
)
{
  wssl_ssize_t size;

  *processed = 0;
  switch(client->state)
  {
    case WSSL_CLIENT_STATE_WAIT_METHOD:
      size = wssl_client_processing_find_word((char*)&client->input_buffer.data[data_begin], data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_method(&client->header, (char*)&client->input_buffer.data[data_begin], size));
        *processed = size;
        client->state = WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR;
      }
      else if(size <= 0)
        *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR:
      size = wssl_client_processing_count_spaces((char*)&client->input_buffer.data[data_begin], data_size);
      if(size > 0 && size < data_size)
      {
        *processed = size;
        client->state = WSSL_CLIENT_STATE_WAIT_URI;
      }
      else if(size <= 0)
        *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_URI:
      size = wssl_client_processing_find_word((char*)&client->input_buffer.data[data_begin], data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_uri(&client->header, (char*)&client->input_buffer.data[data_begin], size));
        *processed = size;
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR;
      }
      else if(size <= 0)
        *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR:
      size = wssl_client_processing_count_spaces((char*)&client->input_buffer.data[data_begin], data_size);
      if(size > 0 && size < data_size)
      {
        *processed = size;
        client->state = WSSL_CLIENT_STATE_WAIT_VERSION;
      }
      else if(size <= 0)
        *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION:
      size = wssl_client_processing_find_word((char*)&client->input_buffer.data[data_begin], data_size);
      if(size > 0 && size < data_size)
      {
        WSSL_CALL(wssl_header_insert_version(&client->header, (char*)&client->input_buffer.data[data_begin], size));
        *processed = size;
        client->state = WSSL_CLIENT_STATE_WAIT_CRLF;
      }
      else if(size <= 0)
        *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_CRLF:
      if(data_size > 2)
        if
        (
          (char)client->input_buffer.data[data_begin] == '\r' &&
          (char)client->input_buffer.data[data_begin+1] == '\n'
        )
        {
          *processed = 2;
          client->state = WSSL_CLIENT_STATE_WAIT_FIELD_KEY;
        }
        else
          *processed = -1;
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_KEY:
      /*#*/
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR:
      /*#*/
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE:
      /*#*/
      break;
    case WSSL_CLIENT_STATE_WAIT_FRAME:
      /*#*/
      break;
    case WSSL_CLIENT_STATE_DELETE:
      /*#*/
      break;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
