#ifndef _WSSL_G_H_
#define _WSSL_G_H_

#define MAKE_WSSL_SET(what_type, what_name) \
static inline                               \
void wssl_set_##what_name                   \
(                                           \
  _WSSL_MODIFY_ wssl_t*   wssl,             \
  _WSSL_IN_     what_type what_name         \
)                                           \
{                                           \
  wssl->what_name = what_name;              \
}                                           \

_INCLUDE_BEGIN_

MAKE_WSSL_SET(char*,                                 server_name)
MAKE_WSSL_SET(wssl_size_t,                           buffer_size_in_octets)
MAKE_WSSL_SET(wssl_ssize_t,                          sleep_in_mseconds)
MAKE_WSSL_SET(wssl_size_t,                           max_number_of_output_chunks)
MAKE_WSSL_SET(wssl_size_t,                           max_number_of_received_multi_frames)
MAKE_WSSL_SET(void*,                                 global_extra_data)
MAKE_WSSL_SET(wssl_connect_callback_f*,              connect_callback)
MAKE_WSSL_SET(wssl_disconnect_callback_f*,           disconnect_callback)
MAKE_WSSL_SET(wssl_header_callback_f*,               header_callback)
MAKE_WSSL_SET(wssl_begin_callback_f*,                begin_callback)
MAKE_WSSL_SET(wssl_receive_text_frame_callback_f*,   receive_text_frame_callback)
MAKE_WSSL_SET(wssl_receive_binary_frame_callback_f*, receive_binary_frame_callback)
MAKE_WSSL_SET(wssl_receive_close_frame_callback_f*,  receive_close_frame_callback)
MAKE_WSSL_SET(wssl_receive_ping_frame_callback_f*,   receive_ping_frame_callback)
MAKE_WSSL_SET(wssl_receive_pong_frame_callback_f*,   receive_pong_frame_callback)
MAKE_WSSL_SET(wssl_tick_callback_f*,                 tick_callback)

_INCLUDE_END_

#endif
