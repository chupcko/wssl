#ifndef _INLINE_H_
#define _INLINE_H_

_INCLUDE_BEGIN_

#define MAKE_SET(what_type, what_name)  \
static inline void wssl_set_##what_name \
(                                       \
  _WSSL_MODIFY_ wssl_t*   wssl,         \
  _WSSL_IN_     what_type what_name     \
)                                       \
{                                       \
  wssl->what_name = what_name;          \
}                                       \

MAKE_SET(wssl_size_t,                           buffer_size_in_octets)
MAKE_SET(wssl_ssize_t,                          epoll_sleep_in_mseconds)
MAKE_SET(void*,                                 global_extra_data)
MAKE_SET(wssl_connect_callback_f*,              connect_callback)
MAKE_SET(wssl_disconnect_callback_f*,           disconnect_callback)
MAKE_SET(wssl_header_callback_f*,               header_callback)
MAKE_SET(wssl_receive_text_frame_callback_f*,   receive_text_frame_callback)
MAKE_SET(wssl_receive_binary_frame_callback_f*, receive_binary_frame_callback)
MAKE_SET(wssl_receive_close_frame_callback_f*,  receive_close_frame_callback)
MAKE_SET(wssl_receive_ping_frame_callback_f*,   receive_ping_frame_callback)
MAKE_SET(wssl_receive_pong_frame_callback_f*,   receive_pong_frame_callback)
MAKE_SET(wssl_tick_callback_f*,                 tick_callback)
#undef MAKE_SET

_INCLUDE_END_

static inline const char* wssl_get_bool_string
(
  _WSSL_IN_ const bool value
)
{
  if(value)
    return "true";
  return "false";
}

static inline void wssl_frame_fill
(
  _WSSL_MODIFY_       wssl_t*       wssl,
  _WSSL_MODIFY_       wssl_frame_t* frame,
  _WSSL_IN_     const wssl_octet_t  opcode,
  _WSSL_IN_     const bool          masked,
  _WSSL_IN_           wssl_octet_t* payload,
  _WSSL_IN_     const wssl_size_t   patloay_size
)
{
  frame->fin          = true;
  frame->opcode       = opcode;
  frame->masked       = masked;
  frame->length       = 0;
  frame->payload      = payload;
  frame->payload_size = patloay_size;
  if(masked)
  {
    wssl_size_t i;
    for(i = 0; i < WSSL_FRAME_MASKING_KEY_SIZE; i++)
      frame->masking_key[i] = (wssl_octet_t)(rand_r(&wssl->random_seed)&0xff);
  }
}

static inline void wssl_client_set_for_disconnecting
(
  _WSSL_MODIFY_ wssl_client_t*                  client,
  _WSSL_IN_     wssl_client_disconnect_reason_e disconnect_reason
)
{
  client->state = WSSL_CLIENT_STATE_FOR_DISCONNECTING;
  client->disconnect_reason = disconnect_reason;
  wssl_chain_delete_link(&client->chain_link);
  wssl_chain_add_link_backward(&client->wssl->clients_for_disconnecting, &client->chain_link);
}

static inline wssl_result_t wssl_client_epoll_event_add_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events |= EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline wssl_result_t wssl_client_epoll_event_delete_out
(
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  client->epoll_event.events &= ~EPOLLOUT;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_MOD, client->socket_descriptor, &client->epoll_event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

#endif
