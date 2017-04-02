#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_string(client, &WSSL_MAKE_STRING(data, data_size));
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 5001));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
