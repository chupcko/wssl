#include "main.h"

_LIBRARY_FUNCTION_
void wssl_client_dump
(
  _WSSL_IN_ const wssl_client_t* client,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  fprintf(file, INDENT_FORMAT "id: ", INDENT(indent_level));
  wssl_id_dump(&client->id, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "socket_descriptor: %d\n"
      INDENT_FORMAT "ip: %s\n"
      INDENT_FORMAT "port: %d\n"
      INDENT_FORMAT "input_buffer:\n"
    ),
    INDENT(indent_level), client->socket_descriptor,
    INDENT(indent_level), client->ip,
    INDENT(indent_level), client->port,
    INDENT(indent_level)
  );
  wssl_buffer_dump(&client->input_buffer, file, indent_level+1);
  fprintf(file, INDENT_FORMAT "output_buffer:\n", INDENT(indent_level));
  wssl_buffer_dump(&client->output_buffer, file, indent_level+1);
  fprintf(file, INDENT_FORMAT "state: %s\n", INDENT(indent_level), wssl_get_client_state(client->state));
  wssl_header_dump(&client->header, file, indent_level+1);
}
