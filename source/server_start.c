#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_start
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  server->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(server->socket_descriptor < 0)
    return WSSL_MAKE_RESULT_ERRNO("socket", errno);

  int socket_reusable = 1;
  if(setsockopt(server->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_reusable, sizeof socket_reusable) < 0)
    return WSSL_MAKE_RESULT_ERRNO("setsockopt", errno);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(server->ip);
  server_address.sin_port = htons(server->port);
  if(bind(server->socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr)) < 0)
    return WSSL_MAKE_RESULT_ERRNO("bind", errno);

  if(listen(server->socket_descriptor, SERVER_BACKLOG_SIZE) < 0)
    return WSSL_MAKE_RESULT_ERRNO("listen", errno);

  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.ptr = (void*)&server->epoll_data;
  if(epoll_ctl(server->wssl->epoll_descriptor, EPOLL_CTL_ADD, server->socket_descriptor, &event) < 0)
    return WSSL_MAKE_RESULT_ERRNO("epoll_ctl", errno);

  return WSSL_MAKE_RESULT_OK;
}
