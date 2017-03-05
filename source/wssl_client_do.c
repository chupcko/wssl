#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_do
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_client_t* client
)
{
  ssize_t recv_size;
char a[10+1];

  recv_size = recv
  (
    client->socket_descriptor,
    a,
    10,
    0
  );
  if(recv_size < 0)
  {
    if(errno == ECONNRESET || errno == EPIPE)
    {
      WSSL_CALL(wssl_client_delete(wssl, client));
      return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
    }
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, "recv", errno);
  }
  if(recv_size == 0)
  {
    WSSL_CALL(wssl_client_delete(wssl, client));
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
  }

/*#*/
a[recv_size] = '\0';
printf("\"%s\"\n", a);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
