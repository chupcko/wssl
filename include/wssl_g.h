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

MAKE_WSSL_SET(char*,                                      server_name                        )
MAKE_WSSL_SET(wssl_size_t,                                buffer_size_in_octets              )
MAKE_WSSL_SET(wssl_ssize_t,                               sleep_in_mseconds                  )
MAKE_WSSL_SET(wssl_size_t,                                max_number_of_output_chunks        )
MAKE_WSSL_SET(wssl_size_t,                                max_number_of_received_multi_frames)
MAKE_WSSL_SET(void*,                                      global_extra_data                  )
MAKE_WSSL_SET(wssl_on_connect_callback_f*,                on_connect_callback                )
MAKE_WSSL_SET(wssl_on_disconnect_callback_f*,             on_disconnect_callback             )
MAKE_WSSL_SET(wssl_on_receive_header_callback_f*,         on_receive_header_callback         )
MAKE_WSSL_SET(wssl_on_start_receiving_frames_callback_f*, on_start_receiving_frames_callback )
MAKE_WSSL_SET(wssl_on_receive_text_frame_callback_f*,     on_receive_text_frame_callback     )
MAKE_WSSL_SET(wssl_on_receive_binary_frame_callback_f*,   on_receive_binary_frame_callback   )
MAKE_WSSL_SET(wssl_on_receive_close_frame_callback_f*,    on_receive_close_frame_callback    )
MAKE_WSSL_SET(wssl_on_receive_ping_frame_callback_f*,     on_receive_ping_frame_callback     )
MAKE_WSSL_SET(wssl_on_receive_pong_frame_callback_f*,     on_receive_pong_frame_callback     )
MAKE_WSSL_SET(wssl_on_tick_callback_f*,                   on_tick_callback                   )

_INCLUDE_END_

#endif
