#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  wssl_client_send_string(client, &WSSL_MAKE_STRING(data, data_size));
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    fprintf(stderr, "usage: %s certificate_file private_key_file\n", argv[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);

  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "0.0.0.0", 5011, WSSL_NO_IPV6, argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "::",      5111, WSSL_IPV6,    argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
