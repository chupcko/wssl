#include "main.h"

_LIBRARY_FUNCTION_
void wssl_server_dump
(
  _WSSL_IN_ const wssl_server_t* server,
  _WSSL_IN_       FILE*          file,
  _WSSL_IN_ const wssl_size_t    indent_level
)
{
  wssl_chain_t* client_link;
  wssl_client_t* client;

  fprintf
  (
    file,
    (
      INDENT_FORMAT "ip: %s\n"
      INDENT_FORMAT "port: %d\n"
      INDENT_FORMAT "socket_descriptor: %d\n"
    ),
    INDENT(indent_level), server->ip,
    INDENT(indent_level), server->port,
    INDENT(indent_level), server->socket_descriptor
  );
  if(wssl_chain_is_not_empty(&server->clients))
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(client_link, &server->clients)
    {
      client = (wssl_client_t*)client_link;
      fprintf(file, INDENT_FORMAT "client:\n", INDENT(indent_level));
      wssl_client_dump(client, file, indent_level+1);
    }
  else
    fprintf(file, INDENT_FORMAT "no clients\n", INDENT(indent_level));
}
