#ifndef _REST_H_
#define _REST_H_

_INCLUDE_BEGIN_

static inline
void wssl_result_dump
(
  _WSSL_IN_ const wssl_result_t result,
  _WSSL_IN_       FILE*         file
)
{
  fprintf
  (
    file,
    "\"%s\" on \"%s\" with %d:\"%s\"\n",
    wssl_result_get_code_string(result),
    result.where,
    result.last_errno,
    strerror(result.last_errno)
  );
}

static inline
void* wssl_client_get_global_extra_data
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->wssl->global_extra_data;
}

_INCLUDE_END_

static inline
const char* wssl_get_bool_string
(
  _WSSL_IN_ const bool value
)
{
  if(value)
    return "true";
  return "false";
}

static inline
void wssl_client_set_for_disconnecting
(
  _WSSL_MODIFY_ wssl_client_t*                  client,
  _WSSL_IN_     wssl_client_disconnect_reason_e disconnect_reason
)
{
  client->state = WSSL_CLIENT_STATE_FOR_DISCONNECTING;
  client->disconnect_reason = disconnect_reason;
  wssl_client_chain_delete_link(&client->chain_link);
  wssl_client_chain_add_link_backward(&client->wssl->clients_for_disconnecting, &client->chain_link);
}

#endif
