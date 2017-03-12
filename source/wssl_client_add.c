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
  if(client == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "client", 0);

  client->id = wssl_get_next_client_id(wssl);

  client_address_length = (socklen_t)sizeof client_address;
  client->socket_descriptor = accept(server->socket_descriptor, (struct sockaddr*)&client_address, &client_address_length);
  if(client->socket_descriptor  < 0)
  {
    int last_errno = errno;

    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "accept", last_errno);
  }

  inet_ntop(AF_INET, (void*)&client_address.sin_addr, client->ip, WSSL_IP_SIZE);
  client->port = ntohs(client_address.sin_port);

  client->epoll.type = WSSL_EPOLL_TYPE_CLIENT;
  client->epoll.client = client;

  client->local_extra_data = WSSL_NULL;

  wssl_buffer_init(&client->input_buffer);
  wssl_buffer_init(&client->output_buffer);

  client->state = WSSL_CLIENT_STATE_WAIT_METHOD;
  wssl_header_init(&client->header);

  client->wssl = wssl;
  client->server = server;

  event.events = EPOLLIN;
  event.data.ptr = (void*)&client->epoll;
  if(epoll_ctl(wssl->epoll_descriptor, EPOLL_CTL_ADD, client->socket_descriptor, &event) < 0)
  {
    int last_errno = errno;

    close(client->socket_descriptor);
    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", last_errno);
  }

  wssl_chain_add_link_backward(&server->clients, &client->chain_link);

  if(wssl->connect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    (*wssl->connect_callback_function)(client);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}
