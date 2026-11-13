#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_processing_frame
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  wssl_frame_t frame;

  client->number_of_received_multi_frames = 0;
  switch(client->frame.opcode)
  {
    case FRAME_OPCODE_TEXT:
    {
      if(client->wssl->on_receive_text_frame_callback != WSSL_CALLBACK_NONE)
      {
        TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_receive_text_frame_callback)(client, (char*)client->frame.payload, client->frame.payload_size));
      }
      break;
    }
    case FRAME_OPCODE_BINARY:
    {
      if(client->wssl->on_receive_binary_frame_callback != WSSL_CALLBACK_NONE)
      {
        TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_receive_binary_frame_callback)(client, client->frame.payload, client->frame.payload_size));
      }
      break;
    }
    case FRAME_OPCODE_CLOSE:
    {
      if(client->wssl->on_receive_close_frame_callback != WSSL_CALLBACK_NONE)
      {
        TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_receive_close_frame_callback)(client));
      }
      wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_CLOSE, false, WSSL_NULL, 0);
      TRY_CALL(wssl_client_send_frame(client, &frame));
      PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
      MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_REQUESTED);
      break;
    }
    case FRAME_OPCODE_PING:
    {
      if(client->wssl->on_receive_ping_frame_callback != WSSL_CALLBACK_NONE)
      {
        TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_receive_ping_frame_callback)(client, client->frame.payload, client->frame.payload_size));
      }
      wssl_frame_fill(client->wssl, &frame, FRAME_OPCODE_PONG, false, client->frame.payload, client->frame.payload_size);
      TRY_CALL(wssl_client_send_frame(client, &frame));
      PASS_IF_CLIENT_IS_MARKED_FOR_DISCONNECTING(client);
      break;
    }
    case FRAME_OPCODE_PONG:
    {
      if
      (
        client->ping_in_progress == true &&
        client->frame.payload_size == WSSL_PING_CONTEXT_SIZE &&
        memcmp(client->frame.payload, client->ping_context, WSSL_PING_CONTEXT_SIZE) == 0
      )
      {
        client->last_pong_in_mseconds = wssl_now_in_mseconds();
        client->ping_in_progress = false;
      }
      if(client->wssl->on_receive_pong_frame_callback != WSSL_CALLBACK_NONE)
      {
        TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_receive_pong_frame_callback)(client, client->frame.payload, client->frame.payload_size));
      }
      break;
    }
    default:
    {
      MARK_CLIENT_FOR_DISCONNECTING_THEN_PASS(client, WSSL_CLIENT_DISCONNECT_REASON_BAD_FRAME_OPCODE);
      break;
    }
  }

  PASS;
}
