#include "main.h"

_LIBRARY_FUNCTION_
void wssl_for_each_client_call
(
  _WSSL_MODIFY_ wssl_t*                          wssl,
  _WSSL_IN_     wssl_for_each_client_callback_f* callback,
  _WSSL_MODIFY_ void*                            local_extra_data
)
{
  wssl_server_chain_t* server_link;
  wssl_client_chain_t* client_link;
  wssl_client_t* client;
  WSSL_CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(client_link, &wssl_server_chain_entry(server_link)->clients)
    {
      client = wssl_client_chain_entry(client_link);
      if(wssl_client_is_in_frame_processing(client))
        (*callback)(client, local_extra_data);
    }
}
