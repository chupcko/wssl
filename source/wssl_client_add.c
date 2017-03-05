#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_add
(
  _WSSL_MODIFY_ wssl_t*        wssl,
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  wssl_client_t* client;
  struct sockaddr_in client_address;
  socklen_t client_address_length;
  struct epoll_event event;

  client = (wssl_client_t*)malloc(sizeof(wssl_client_t));
  if(client == (wssl_client_t*)NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_MEMORY, "client", 0);

  client_address_length = (socklen_t)sizeof client_address;
  client->socket_descriptor = accept(server->socket_descriptor, (struct sockaddr*)&client_address, &client_address_length);
  if(client->socket_descriptor  < 0)
  {
    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, "accept", errno);
  }

  inet_ntop(AF_INET, (void*)&client_address.sin_addr, client->ip, WSSL_IP_SIZE);
  client->port = ntohs(client_address.sin_port);

  client->epoll.type = WSSL_EPOLL_TYPE_CLIENT;
  client->epoll.client = client;

  client->connection_info.wssl = wssl;
  client->connection_info.server = server;
  client->connection_info.client = client;
  client->connection_info.local_extra_data = NULL;

  event.events = EPOLLIN;
  event.data.ptr = (void*)&client->epoll;
  if(epoll_ctl(wssl->epoll_descriptor, EPOLL_CTL_ADD, client->socket_descriptor, &event) < 0)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERRNO, "epoll_ctl", errno);

  wssl_chain_add_link_backward(&server->clients, &client->chain_link);

  if(wssl->connect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    (*wssl->connect_callback_function)(&client->connection_info);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
