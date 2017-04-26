#include <stdlib.h>

#include "wssl.h"

int main(void)
{
  WSSL_DECLARE(wssl);

  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 6001, WSSL_NO_SSL, WSSL_NO_IPV6));
  WSSL_TRY_CALL(wssl_server_add(&wssl, "::",      6101, WSSL_NO_SSL, WSSL_IPV6));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
