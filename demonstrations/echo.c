#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_text(client, data);
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  WSSL_TRY(wssl_server_add(&wssl, "0.0.0.0", 5001));
  WSSL_TRY(wssl_loop(&wssl));
  WSSL_TRY(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
