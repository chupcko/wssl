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
  wssl_id_dump(&client->id, file);
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
      INDENT_FORMAT "local_extra_data: "
    ),
    INDENT(indent_level+1), client->ip,
    INDENT(indent_level+1), client->port,
    INDENT(indent_level+1)
  );
  if(client->local_extra_data != WSSL_NULL)
    fprintf(file, "%p", client->local_extra_data);
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
  fprintf(file, INDENT_FORMAT "output_buffer:", INDENT(indent_level+1));
  if(wssl_buffer_is_allocated(&client->output_buffer))
  {
    fprintf(file, "\n");
    wssl_buffer_dump(&client->output_buffer, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
  fprintf
  (
    file,
    (
      INDENT_FORMAT "state: \"%s\"\n"/*# dodaj delete reason */
      INDENT_FORMAT "header:\n"/*# dodaj frame*/
    ),
    INDENT(indent_level+1), wssl_client_get_state_string(client->state),
    INDENT(indent_level+1)
  );
  wssl_header_dump(&client->header, file, indent_level+2);
}
