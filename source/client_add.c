#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_add
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  wssl_client_t* client = (wssl_client_t*)malloc(sizeof(wssl_client_t));
  if(client == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "client", 0);

  client->server = server;
  client->wssl = server->wssl;

  client->id = wssl_get_next_client_id(client->wssl);

  struct sockaddr_in client_address;
  socklen_t client_address_length = (socklen_t)sizeof client_address;
  client->socket_descriptor = accept(server->socket_descriptor, (struct sockaddr*)&client_address, &client_address_length);
  if(client->socket_descriptor  < 0)
  {
    int last_errno = errno;

    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "accept", last_errno);
  }

  inet_ntop(AF_INET, (void*)&client_address.sin_addr, client->ip, WSSL_IP_SIZE_IN_CHAR);
  client->port = ntohs(client_address.sin_port);

  int fcntl_flags = fcntl(client->socket_descriptor, F_GETFL, 0);
  if
  (
    fcntl_flags < 0 ||
    fcntl(client->socket_descriptor, F_SETFL, fcntl_flags|O_NONBLOCK) < 0
  )
  {
    int last_errno = errno;

    close(client->socket_descriptor);
    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "fcntl", last_errno);
  }

  client->epoll_data.type = WSSL_EPOLL_DATA_TYPE_CLIENT;
  client->epoll_data.client = client;
  client->epoll_event.events = EPOLLIN;
  client->epoll_event.data.ptr = (void*)&client->epoll_data;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_ADD, client->socket_descriptor, &client->epoll_event) < 0)
  {
    int last_errno = errno;

    close(client->socket_descriptor);
    free((void*)client);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_ERRNO, "epoll_ctl", last_errno);
  }

  client->local_extra_data = WSSL_NULL;

  wssl_buffer_init(&client->input_buffer);
  wssl_buffer_init(&client->output_buffer);

  client->state = WSSL_CLIENT_STATE_WAIT_METHOD;
  client->disconnect_reason = WSSL_CLIENT_DISCONNECT_REASON_NONE;

  wssl_header_init(&client->header);
  wssl_frame_init(&client->frame);

  wssl_chain_add_link_backward(&server->clients, &client->chain_link);

  if(client->wssl->connect_callback != WSSL_CALLBACK_NONE)
    (*client->wssl->connect_callback)(client);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}
