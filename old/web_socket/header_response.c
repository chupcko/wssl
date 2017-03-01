#include "main.h"

bool header_response(int client_socket_fd, header_data_t header_data[])
{
  bool result;
  int out_buffer_length;

  if
  (
    header_data[HEADER_INDEX_GET].value_length != -1 &&
    header_data[HEADER_INDEX_HOST].value_length != -1 &&
    header_data[HEADER_INDEX_UPGRADE].value_length != -1 &&
    header_data[HEADER_INDEX_SEC_WS_KEY].value_length != -1
  )
  {
    char sec_ws_accept[HANDSHAKE_CALCULATION_RESULT_SIZE];

    if(handshake_calculate(header_data[HEADER_INDEX_SEC_WS_KEY].value, sec_ws_accept, HANDSHAKE_CALCULATION_RESULT_SIZE) < 0)
      error("Cannot handshake");
    out_buffer_length = snprintf
    (
      Out_buffer,
      BUFFER_SIZE,
      (
        "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n"
        "Server: " SERVER_NAME "\r\n"
        "\r\n"
      ),
      sec_ws_accept
    );
    result = true;
  }
  else
  {
    out_buffer_length = snprintf
    (
      Out_buffer,
      BUFFER_SIZE,
      (
        "HTTP/1.1 400 Bad Request\r\n"
        "Server: " SERVER_NAME "\r\n"
        "\r\n"
      )
    );
    result = false;
  }

  client_send(client_socket_fd, Out_buffer, out_buffer_length);
  return result;
}
