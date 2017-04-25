#include "main.h"

_FUNCTION_
wssl_result_t wssl_server_start
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  if(server->ipv6)
    server->socket_descriptor = socket(AF_INET6, SOCK_STREAM, 0);
  else
    server->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(server->socket_descriptor < 0)
    FAIL_ERRNO("socket", errno);

  int socket_reusable = 1;
  if(setsockopt(server->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_reusable, sizeof socket_reusable) < 0)
    FAIL_ERRNO("setsockopt", errno);

  int return_value;
  if(server->ipv6)
  {
    char ip_copy[WSSL_IP_SIZE_IN_CHAR];
    wssl_str_copy(ip_copy, WSSL_IP_SIZE_IN_CHAR, server->ip);
    char* device = strchr(ip_copy, '%');
    if(device != NULL)
    {
      *device = '\0';
      device++;
    }

    struct sockaddr_in6 server_address;
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(server->port);
    server_address.sin6_flowinfo = 0;
    return_value = inet_pton(AF_INET6, ip_copy, (void*)&server_address.sin6_addr);
    if(return_value < 0)
      FAIL_ERRNO("inet_pton", errno);
    if(return_value == 0)
      FAIL_ERROR("server_ip", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
    if(device != NULL)
    {
      server_address.sin6_scope_id = if_nametoindex(device);
      if(server_address.sin6_scope_id == 0)
        FAIL_ERRNO("if_nametoindex", errno);
    }
    else
      server_address.sin6_scope_id = 0;

    if(bind(server->socket_descriptor, (struct sockaddr*)&server_address, sizeof server_address) < 0)
      FAIL_ERRNO("bind", errno);
  }
  else
  {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server->port);
    return_value = inet_pton(AF_INET, server->ip, (void*)&server_address.sin_addr);
    if(return_value < 0)
      FAIL_ERRNO("inet_pton", errno);
    if(return_value == 0)
      FAIL_ERROR("server_ip", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);

    if(bind(server->socket_descriptor, (struct sockaddr*)&server_address, sizeof server_address) < 0)
      FAIL_ERRNO("bind", errno);
  }

  if(listen(server->socket_descriptor, SERVER_BACKLOG_SIZE) < 0)
    FAIL_ERRNO("listen", errno);

  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.ptr = (void*)&server->epoll_data;
  if(epoll_ctl(server->wssl->epoll_descriptor, EPOLL_CTL_ADD, server->socket_descriptor, &event) < 0)
    FAIL_ERRNO("epoll_ctl", errno);

  PASS;
}
