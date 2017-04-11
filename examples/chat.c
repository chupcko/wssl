#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

void for_client(wssl_client_t* client, void* local_extra_data)
{
  wssl_client_send_text(client, (char*)local_extra_data);
}

#define OUTPUT_SIZE 1024

void on_start_receiving_frames(wssl_client_t* client)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = JOIN", client->header.uri.data, client->id.suffix);
  wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
}

void on_receive_text_frame(wssl_client_t* client, char* data, wssl_size_t data_size)
{
  char output[OUTPUT_SIZE];
  snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " - %s", client->header.uri.data, client->id.suffix, data);
  wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
}

void on_disconnect(wssl_client_t* client, wssl_client_disconnect_reason_e disconnect_reason)
{
  if(wssl_client_disconnect_reason_is_not_error(disconnect_reason))
  {
    char output[OUTPUT_SIZE];
    snprintf(output, OUTPUT_SIZE, "%s:%" WSSL_PRINT_ID_SUFFIX " = LEAVE", client->header.uri.data, client->id.suffix);
    wssl_for_each_client_in_frame_processing_call(wssl_client_get_wssl(client), &for_client, (void*)output);
  }
}

int main(void)
{
  WSSL_DECLARE(wssl);

  wssl_set_on_start_receiving_frames_callback(&wssl, &on_start_receiving_frames);
  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_on_disconnect_callback(&wssl, &on_disconnect);

  WSSL_TRY_CALL(wssl_server_add(&wssl, "0.0.0.0", 5003));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
