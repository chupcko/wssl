#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_add
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  wssl_client_t* client = (wssl_client_t*)malloc(sizeof(wssl_client_t));
  if(client == NULL)
    FAIL_ERROR("client", WSSL_RESULT_CODE_ERROR_NO_MEMORY);

  client->server = server;
  client->wssl = server->wssl;

  wssl_set_next_client_id(client->wssl, &client->id);

  if(server->ipv6)
  {
    struct sockaddr_in6 client_address;
    socklen_t client_address_length = (socklen_t)sizeof client_address;
    client->socket_descriptor = accept(server->socket_descriptor, (struct sockaddr*)&client_address, &client_address_length);
    if(client->socket_descriptor  < 0)
    {
      int system_errno = errno;
      free((void*)client);
      FAIL_ERRNO("accept", system_errno);
    }

    inet_ntop(AF_INET6, (void*)&client_address.sin6_addr, client->ip, WSSL_IP_SIZE_IN_CHAR);
    client->port = ntohs(client_address.sin6_port);
  }
  else
  {
    struct sockaddr_in client_address;
    socklen_t client_address_length = (socklen_t)sizeof client_address;
    client->socket_descriptor = accept(server->socket_descriptor, (struct sockaddr*)&client_address, &client_address_length);
    if(client->socket_descriptor  < 0)
    {
      int system_errno = errno;
      free((void*)client);
      FAIL_ERRNO("accept", system_errno);
    }

    inet_ntop(AF_INET, (void*)&client_address.sin_addr, client->ip, WSSL_IP_SIZE_IN_CHAR);
    client->port = ntohs(client_address.sin_port);
  }

  int fcntl_flags = fcntl(client->socket_descriptor, F_GETFL, 0);
  if
  (
    fcntl_flags < 0 ||
    fcntl(client->socket_descriptor, F_SETFL, fcntl_flags|O_CLOEXEC|O_NONBLOCK) < 0
  )
  {
    int system_errno = errno;
    close(client->socket_descriptor);
    free((void*)client);
    FAIL_ERRNO("fcntl", system_errno);
  }

  client->epoll_data.type = WSSL_EPOLL_DATA_TYPE_CLIENT;
  client->epoll_data.client = client;
  client->epoll_event.events = EPOLLIN;
  client->epoll_event.data.ptr = (void*)&client->epoll_data;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_ADD, client->socket_descriptor, &client->epoll_event) < 0)
  {
    int system_errno = errno;

    close(client->socket_descriptor);
    free((void*)client);
    FAIL_ERRNO("epoll_ctl", system_errno);
  }

  client->connection_extra_data = WSSL_NULL;

  wssl_buffer_init(&client->input_buffer);
  wssl_chunk_chain_root_init(&client->output_chunks);
  client->number_of_output_chunks = 0;

  client->state = WSSL_CLIENT_STATE_WAIT_METHOD;
  client->disconnect_reason = WSSL_CLIENT_DISCONNECT_REASON_NONE;

  wssl_header_init(&client->header);
  wssl_frame_init(&client->frame);
  client->number_of_received_multi_frames = 0;

  wssl_client_chain_add_link_backward(&server->clients, &client->server_chain_link);
  wssl_client_chain_add_link_backward(&server->wssl->clients_in_wait_header, &client->wssl_chain_link);

  if(client->wssl->on_connect_callback != WSSL_CALLBACK_NONE)
    (*client->wssl->on_connect_callback)(client);

  PASS;
}
