#include <stdlib.h>

#include "wssl.h"

int main(void)
{
  WSSL_DECLARE(wssl);

  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
