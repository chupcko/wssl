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
  CHAIN_FOR_EACH_LINK_FORWARD(client_link, &wssl->clients_in_frame_processing)
    (*callback)(wssl_client_chain_get_entry_from_wssl_chain_link(client_link), local_extra_data);
}
