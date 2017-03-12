#include "main.h"

_LIBRARY_FUNCTION_
void wssl_dump
(
  _WSSL_IN_ const wssl_t*     wssl,
  _WSSL_IN_       FILE*       file,
  _WSSL_IN_ const wssl_size_t indent_level
)
{
  wssl_chain_t* server_link;
  wssl_server_t* server;

  if(wssl_chain_is_not_empty(&wssl->servers))
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    {
      server = (wssl_server_t*)server_link;
      fprintf(file, INDENT_FORMAT "server:\n", INDENT(indent_level));
      wssl_server_dump(server, file, indent_level+1);
    }
  else
    fprintf(file, INDENT_FORMAT "no servers\n", INDENT(indent_level));
  fprintf(file, INDENT_FORMAT "next_client_id: ", INDENT(indent_level));
  wssl_id_dump(&wssl->next_client_id, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "epoll_descriptor: %d\n"
      INDENT_FORMAT "global_extra_data: %p\n"
      INDENT_FORMAT "connect_callback_function: %p\n"
      INDENT_FORMAT "disconnect_callback_function: %p\n"
      INDENT_FORMAT "receiving_callback_function: %p\n"
      INDENT_FORMAT "tick_callback_function: %p\n"
    ),
    INDENT(indent_level), wssl->epoll_descriptor,
    INDENT(indent_level), wssl->global_extra_data,
    INDENT(indent_level), wssl->connect_callback_function,
    INDENT(indent_level), wssl->disconnect_callback_function,
    INDENT(indent_level), wssl->receiving_callback_function,
    INDENT(indent_level), wssl->tick_callback_function
  );
}
