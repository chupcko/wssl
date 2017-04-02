#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wssl.h"

#define OUTPUT_SIZE 64

void on_client(wssl_client_t* client, void* local_extra_data)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "t=%ld", (long)time(NULL));
  wssl_client_send_text(client, output);
}

bool on_tick(wssl_t* wssl)
{
  wssl_for_each_client_call(wssl, &on_client, NULL);
  return true;
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_sleep_in_mseconds(&wssl, 500);
  wssl_set_tick_callback(&wssl, &on_tick);

  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 5002));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
