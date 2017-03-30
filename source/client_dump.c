#include "main.h"

_LIBRARY_FUNCTION_
void wssl_client_dump
(
  _WSSL_IN_ const wssl_client_t* client,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "client=\n"
      INDENT_FORMAT "id: "
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  wssl_id_print(&client->id, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "socket_descriptor: "
    ),
    INDENT(indent_level+1)
  );
  if(client->socket_descriptor != WSSL_NO_DESCRIPTOR)
    fprintf(file, "%d", client->socket_descriptor);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "ip: %s\n"
      INDENT_FORMAT "port: %d\n"
      INDENT_FORMAT "connection_extra_data: "
    ),
    INDENT(indent_level+1), client->ip,
    INDENT(indent_level+1), client->port,
    INDENT(indent_level+1)
  );
  if(client->connection_extra_data != WSSL_NULL)
    fprintf(file, "%p", client->connection_extra_data);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "input_buffer:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_buffer_is_allocated(&client->input_buffer))
  {
    fprintf(file, "\n");
    wssl_buffer_dump(&client->input_buffer, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
  fprintf(file, INDENT_FORMAT "output_chunks:", INDENT(indent_level+1));
  if(wssl_chunk_chain_is_not_empty(&client->output_chunks))
  {
    fprintf(file, "\n");
    wssl_chunk_chain_t* chunk_link;
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(chunk_link, &client->output_chunks)
      wssl_buffer_dump(&wssl_chunk_chain_entry(chunk_link)->buffer, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
  fprintf
  (
    file,
    (
      INDENT_FORMAT "state: \"%s\"\n"
      INDENT_FORMAT "disconnect_reason: \"%s\"\n"
      INDENT_FORMAT "header:\n"
    ),
    INDENT(indent_level+1), wssl_client_get_state_string(client->state),
    INDENT(indent_level+1), wssl_client_get_disconnect_reason_string(client->disconnect_reason),
    INDENT(indent_level+1)
  );
  wssl_header_dump(&client->header, file, indent_level+2);
  fprintf(file, INDENT_FORMAT "frame:\n", INDENT(indent_level+1));
  wssl_frame_dump(&client->frame, file, indent_level+2);
}
