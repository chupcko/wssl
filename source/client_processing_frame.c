#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_processing_frame
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  wssl_frame_t frame;

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
      if(client->wssl->receive_close_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_close_frame_callback)(client);
      wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_CLOSE, false, WSSL_NULL, 0);
      TRY_CALL(wssl_client_send_frame(client, &frame));
      wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_REQUESTED);
      break;
    case FRAME_OPCODE_PING:
      if(client->wssl->receive_ping_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_ping_frame_callback)(client, client->frame.payload, client->frame.payload_size);
      wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_PONG, false, client->frame.payload, client->frame.payload_size);
      TRY_CALL(wssl_client_send_frame(client, &frame));
      break;
    case FRAME_OPCODE_PONG:
      if(client->wssl->receive_pong_frame_callback != WSSL_CALLBACK_NONE)
        (*client->wssl->receive_pong_frame_callback)(client, client->frame.payload, client->frame.payload_size);
      break;
    defualt:
      wssl_client_set_for_disconnecting(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_FRAME_OPCODE);
      break;
  }

  return MAKE_RESULT_OK;
}
