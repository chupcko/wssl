#include "main.h"

_FUNCTION_
wssl_result_t wssl_client_add
(
  _WSSL_MODIFY_ wssl_server_t* server
)
{
  wssl_client_t* client = (wssl_client_t*)malloc(sizeof(wssl_client_t));
  if(client == NULL)
  {
    FAIL_ERROR("client", WSSL_RESULT_CODE_ERROR_NO_MEMORY);
  }

  client->server = server;
  client->wssl = server->wssl;

  if(server->configuration.ipv6 == true)
  {
    struct sockaddr_in6 client_address;
    socklen_t client_address_length = (socklen_t)sizeof(client_address);
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
    socklen_t client_address_length = (socklen_t)sizeof(client_address);
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

  wssl_set_next_client_id(client->wssl, &client->id);

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

  client->ssl = NULL;
  client->ssl_handshake_done = false;
  client->ssl_want = 0;
  client->ssl_read_blocked = false;

  client->epoll_data.type = WSSL_EPOLL_DATA_TYPE_CLIENT;
  client->epoll_data.client = client;
  if(server->ssl_ctx != NULL)
  {
    client->epoll_event.events = EPOLLIN|EPOLLOUT|EPOLLRDHUP;
  }
  else
  {
    client->epoll_event.events = EPOLLIN|EPOLLRDHUP;
  }
  client->epoll_event.data.ptr = (void*)&client->epoll_data;
  if(epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_ADD, client->socket_descriptor, &client->epoll_event) < 0)
  {
    int system_errno = errno;

    close(client->socket_descriptor);
    free((void*)client);
    FAIL_ERRNO("epoll_ctl", system_errno);
  }

  if(server->ssl_ctx != NULL)
  {
    client->ssl = SSL_new(server->ssl_ctx);
    if(client->ssl == NULL)
    {
      epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_DEL, client->socket_descriptor, NULL);
      close(client->socket_descriptor);
      free((void*)client);
      FAIL_ERROR("SSL_new", WSSL_RESULT_CODE_ERROR_SSL);
    }
    SSL_set_mode(client->ssl, SSL_MODE_ENABLE_PARTIAL_WRITE|SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    SSL_set_quiet_shutdown(client->ssl, 1);
    if(SSL_set_fd(client->ssl, client->socket_descriptor) == 0)
    {
      epoll_ctl(client->wssl->epoll_descriptor, EPOLL_CTL_DEL, client->socket_descriptor, NULL);
      close(client->socket_descriptor);
      SSL_free(client->ssl);
      free((void*)client);
      FAIL_ERROR("SSL_set_fd", WSSL_RESULT_CODE_ERROR_SSL);
    }
    SSL_set_accept_state(client->ssl);
  }

  client->connection_extra_data = WSSL_NULL;

  wssl_buffer_init(&client->input_buffer);
  wssl_chunk_chain_root_init(&client->output_chunks);
  client->number_of_output_chunks = 0;

  client->state = WSSL_CLIENT_STATE_WAIT_METHOD;
  client->disconnect_reason = WSSL_CLIENT_DISCONNECT_REASON_NONE;
  wssl_mseconds_t now_in_mseconds = wssl_now_in_mseconds();
  client->last_receive_in_mseconds = now_in_mseconds;
  client->last_ping_in_mseconds = now_in_mseconds;
  client->last_pong_in_mseconds = now_in_mseconds;
  client->ping_in_progress = false;

  wssl_header_init(&client->header);
  wssl_frame_init(&client->frame);
  client->number_of_received_multi_frames = 0;

  wssl_client_chain_add_link_backward(&server->clients, &client->server_chain_link);
  wssl_client_chain_add_link_backward(&server->wssl->clients_in_wait_header, &client->wssl_chain_link);

  if(client->wssl->on_connect_callback != WSSL_CALLBACK_NONE)
  {
    TRY_CALLBACK_ELSE_MARK_FOR_DISCONNECTING_THEN_PASS(client, (*client->wssl->on_connect_callback)(client));
  }

  PASS;
}
