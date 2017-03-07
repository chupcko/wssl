#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_start
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  int socket_reusable;
  struct sockaddr_in server_address;
  struct epoll_event event;

  server->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(server->socket_descriptor < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "socket", errno);

  socket_reusable = 1;
  if(setsockopt(server->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_reusable, sizeof socket_reusable) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "setsockopt", errno);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(server->ip);
  server_address.sin_port = htons(server->port);
  if(bind(server->socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr)) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "bind", errno);

  if(listen(server->socket_descriptor, WSSL_SERVER_BACKLOG) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "listen", errno);

  event.events = EPOLLIN;
  event.data.ptr = (void*)&server->epoll;
  if(epoll_ctl(wssl->epoll_descriptor, EPOLL_CTL_ADD, server->socket_descriptor, &event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", errno);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
