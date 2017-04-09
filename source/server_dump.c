#include "main.h"

_LIBRARY_FUNCTION_
void wssl_server_dump
(
  _WSSL_IN_     const wssl_server_t* server,
  _WSSL_MODIFY_       FILE*          file,
  _WSSL_IN_     const wssl_size_t    indent_level
)
{
  wssl_client_chain_t* client_link;

  fprintf
  (
    file,
    (
      INDENT_FORMAT "server(%p)=\n"
      INDENT_FORMAT "wssl: %p\n"
      INDENT_FORMAT "ip: %s\n"
      INDENT_FORMAT "port: %d\n"
      INDENT_FORMAT "socket_descriptor: "
    ),
    INDENT(indent_level), (void*)server,
    INDENT(indent_level+1), (void*)server->wssl,
    INDENT(indent_level+1), server->ip,
    INDENT(indent_level+1), server->port,
    INDENT(indent_level+1)
  );
  if(server->socket_descriptor != WSSL_NO_DESCRIPTOR)
    fprintf(file, "%d", server->socket_descriptor);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "clients_in_wait_header:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_client_chain_is_not_empty(&server->clients_in_wait_header))
  {
    fprintf(file, "\n");
    CHAIN_FOR_EACH_LINK_FORWARD(client_link, &server->clients_in_wait_header)
      wssl_client_dump(wssl_client_chain_get_entry_from_chain_link(client_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
  fprintf(file, INDENT_FORMAT "clients_in_frame_processing:", INDENT(indent_level+1));
  if(wssl_client_chain_is_not_empty(&server->clients_in_frame_processing))
  {
    fprintf(file, "\n");
    CHAIN_FOR_EACH_LINK_FORWARD(client_link, &server->clients_in_frame_processing)
      wssl_client_dump(wssl_client_chain_get_entry_from_chain_link(client_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}
