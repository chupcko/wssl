#include "main.h"

static inline wssl_size_t wss_client_processing_find_space
(
  _WSSL_IN_ const char*       data,
  _WSSL_IN_ const wssl_size_t data_size
)
{
  wssl_size_t size;

  for(size = 0; size < data_size; size++)
    if(isspace(data[size]) != 0)
      return size;
  return 0;
}

_FUNCTION_
wssl_result_t wssl_client_processing
(
  _WSSL_MODIFY_       wssl_t*        wssl,
  _WSSL_MODIFY_       wssl_client_t* client,
  _WSSL_IN_     const wssl_size_t    begin,
  _WSSL_IN_     const wssl_size_t    size,
  _WSSL_OUT_          wssl_ssize_t*  processed
)
{
  wssl_size_t data_size;

  *processed = 0;
  switch(client->state)
  {
    case WSSL_CLIENT_STATE_WAIT_METHOD:
      data_size = wss_client_processing_find_space((char*)&client->input_buffer.data[begin], size);
      if(data_size > 0)
      {
        WSSL_CALL(wssl_header_insert_method(&client->header, (char*)&client->input_buffer.data[begin], data_size));
        *processed = data_size;
        client->state = WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR;
      }
      break;
    case WSSL_CLIENT_STATE_WAIT_URI_SEPARATOR:
      break;
    case WSSL_CLIENT_STATE_WAIT_URI:
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION_SEPARATOR:
      break;
    case WSSL_CLIENT_STATE_WAIT_VERSION:
      break;
    case WSSL_CLIENT_STATE_WAIT_CR_LF:
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_KEY:
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE_SEPARATOR:
      break;
    case WSSL_CLIENT_STATE_WAIT_FIELD_VALUE:
      break;
    case WSSL_CLIENT_STATE_WAIT_FRAME:
      break;
    case WSSL_CLIENT_STATE_DELETE:
      break;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
