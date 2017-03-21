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
        (*client->wssl->receive_text_frame_callback)(client, (char*)client->frame.payload, client->frame.payload_size);
      break;
    case FRAME_OPCODE_BINARY:
      if(client->wssl->receive_binary_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_binary_frame_callback)(client, client->frame.payload, client->frame.payload_size);
      break;
    case FRAME_OPCODE_CLOSE:
      /*#
      callback
      WSSL_CALL(wssl_client_send_frame(client, FRAME_OPCODE_CLOSE, WSSL_NULL, 0));
      wssl_client_disconnect(client, WSSL_CLIENT_DISCONNECT_REASON_REQUESTED);
      */
      break;
    case FRAME_OPCODE_PING:
      /*#
      callback
      WSSL_CALL(wssl_client_send_frame(client, FRAME_OPCODE_PONG, client->frame.payload, client->frame.payload_size));
      */
      break;
    case FRAME_OPCODE_PONG:
      /*#
      callback
      */
      break;
    defualt:
      wssl_client_disconnect(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_FRAME_OPCODE);
      break;
  }

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
