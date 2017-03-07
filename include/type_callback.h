#ifndef _TYPE_CALLBACK_H_
#define _TYPE_CALLBACK_H_

_INCLUDE_BEGIN_

struct wssl_t;

typedef void wssl_connect_callback_function_t
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef void wssl_disconnect_callback_function_t
(
  _WSSL_MODIFY_ wssl_client_t*
);

typedef bool wssl_receiving_callback_function_t
(
  _WSSL_MODIFY_ wssl_client_t*,
  _WSSL_IN_     wssl_octet_t*,
  _WSSL_IN_     wssl_size_t
);

typedef bool wssl_tick_callback_function_t
(
  _WSSL_MODIFY_ struct wssl_t*
);

#define WSSL_CALLBACK_FUNCTION_NONE NULL

_INCLUDE_END_

#endif
