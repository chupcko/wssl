#ifndef _REST_H_
#define _REST_H_

_INCLUDE_BEGIN_

static inline
void wssl_try_do
(
  _WSSL_IN_ const wssl_result_t result,
  _WSSL_IN_ const char*         file_name,
  _WSSL_IN_ const unsigned int  line_number,
  _WSSL_IN_ const char*         function_name
)
{
  if(wssl_result_is_not_ok(result))
  {
    fprintf
    (
      stderr,
      "WSSL %s:%u-%s error ",
      file_name,
      line_number,
      function_name
    );
    wssl_result_print(result, stderr);
    fprintf(stderr, "\n");
    abort();
  }
}

#define WSSL_TRY(...) wssl_try_do((__VA_ARGS__), __FILE__, __LINE__, __func__)

static inline
wssl_t* wssl_client_get_wssl
(
  _WSSL_IN_ const wssl_client_t* client
)
{
  return client->wssl;
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
