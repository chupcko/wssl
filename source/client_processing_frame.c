#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_processing_frame
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  switch(client->frame.opcode)
  {
    case FRAME_OPCODE_TEXT:
      if(client->wssl->receive_text_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_text_frame_callback)(client, client->frame.payload, client->frame.payload_size);
      break;
    case FRAME_OPCODE_BINARY:
      if(client->wssl->receive_binary_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_binary_frame_callback)(client, client->frame.payload, client->frame.payload_size);
      break;
    case FRAME_OPCODE_CLOSE:
      /*#*/
      break;
    case FRAME_OPCODE_PING:
      /*#*/
      break;
    case FRAME_OPCODE_PONG:
      /*#*/
      break;
    defualt:
      wssl_client_to_delete(client, WSSL_CLIENT_DELETE_REASON_BAD_FRAME_OPCODE);
      break;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
