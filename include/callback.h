#ifndef _CALLBACK_H_
#define _CALLBACK_H_

_INCLUDE_BEGIN_

typedef void wssl_on_connect_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef void wssl_on_disconnect_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     wssl_client_disconnect_reason_e
);

typedef bool wssl_on_receive_header_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef void wssl_on_start_receiving_frames_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef void wssl_on_receive_text_frame_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     char*,
  _WSSL_IN_     wssl_size_t
);

typedef void wssl_on_receive_binary_frame_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     wssl_octet_t*,
  _WSSL_IN_     wssl_size_t
);

typedef void wssl_on_receive_close_frame_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef void wssl_on_receive_ping_frame_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     wssl_octet_t*,
  _WSSL_IN_     wssl_size_t
);

typedef void wssl_on_receive_pong_frame_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     wssl_octet_t*,
  _WSSL_IN_     wssl_size_t
);

typedef bool wssl_on_tick_callback_f
(
  _WSSL_MODIFY_ struct wssl_t*
);

typedef void wssl_for_each_client_callback_f
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_MODIFY_ void*
);

#define WSSL_CALLBACK_NONE WSSL_NULL

_INCLUDE_END_

#endif
