#include "main.h"

_LIBRARY_FUNCTION_
void wssl_server_dump
(
  _WSSL_IN_ const wssl_server_t* server,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "server=\n"
      INDENT_FORMAT "ip: %s\n"
      INDENT_FORMAT "port: %d\n"
      INDENT_FORMAT "socket_descriptor: "
    ),
    INDENT(indent_level),
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
      INDENT_FORMAT "clients:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_chain_is_not_empty(&server->clients))
  {
    wssl_chain_t* client_link;

    fprintf(file, "\n");
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(client_link, &server->clients)
      wssl_client_dump((wssl_client_t*)client_link, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}