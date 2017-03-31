#include "common.h"

int main(void)
{
  WSSL_DECLARE(wssl);

  WSSL_TRY(wssl_server_add(&wssl, "0.0.0.0", 5000));
  WSSL_TRY(wssl_server_add(&wssl, "0.0.0.0", 6000));
  WSSL_TRY(wssl_loop(&wssl));
  WSSL_TRY(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
