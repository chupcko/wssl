#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wssl.h"

#define OUTPUT_SIZE 64

void for_client(wssl_client_t* client, void* local_extra_data)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "t=%ld", (long)time(NULL));
  wssl_client_send_text(client, output);
}

bool on_tick(wssl_t* wssl)
{
  wssl_for_each_client_in_frame_processing_call(wssl, &for_client, NULL);
  return true;
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    fprintf(stderr, "usage: %s certificate_file private_key_file\n", argv[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 500);
  wssl_set_on_tick_callback(&wssl, &on_tick);

  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "0.0.0.0", 5012, WSSL_NO_IPV6, argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "::",      5112, WSSL_IPV6,    argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
