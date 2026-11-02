#include "main.h"

_LIBRARY_FUNCTION_
void wssl_for_each_client_in_frame_processing_call
(
  _WSSL_MODIFY_ wssl_t*                          wssl,
  _WSSL_IN_     wssl_for_each_client_callback_f* callback,
  _WSSL_MODIFY_ void*                            local_extra_data
)
{
  wssl_client_chain_t* client_link;
  wssl_client_chain_t* client_link_next;
  wssl_client_t* client;
  CHAIN_FOR_EACH_LINK_SAFE_FORWARD(client_link, client_link_next, &wssl->clients_in_frame_processing)
  {
    client = wssl_client_chain_get_entry_from_wssl_chain_link(client_link);
    HONOR_CALLBACK_RESULT(client, (*callback)(client, local_extra_data));
  }
}
