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

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    fprintf(stderr, "usage: %s certificate_file private_key_file\n", argv[0]);
    return EXIT_FAILURE;
  }

  WSSL_DECLARE(wssl);

  wssl_set_on_start_receiving_frames_callback(&wssl, &on_start_receiving_frames);
  wssl_set_on_receive_text_frame_callback(&wssl, &on_receive_text_frame);
  wssl_set_on_disconnect_callback(&wssl, &on_disconnect);

  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "0.0.0.0", 5013, WSSL_NO_IPV6, argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_server_add_ssl(&wssl, "::",      5113, WSSL_IPV6,    argv[1], argv[2]));
  WSSL_TRY_CALL(wssl_loop(&wssl));
  WSSL_TRY_CALL(wssl_clean(&wssl));

  return EXIT_SUCCESS;
}
