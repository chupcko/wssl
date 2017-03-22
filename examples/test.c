#include "common.h"

int main(void)
{
  WSSL_DECLARE(wssl);

  CALL(wssl_server_add(&wssl, "0.0.0.0", 5000));
  CALL(wssl_server_add(&wssl, "0.0.0.0", 6000));
  CALL(wssl_loop(&wssl));
  CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
