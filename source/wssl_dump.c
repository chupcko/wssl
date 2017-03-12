#include "main.h"

_LIBRARY_FUNCTION_
void wssl_dump
(
  _WSSL_IN_ const wssl_t*     wssl,
  _WSSL_IN_       FILE*       file,
  _WSSL_IN_ const wssl_size_t indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "wssl=\n"
      INDENT_FORMAT "epoll_descriptor: "
    ),
    INDENT(indent_level),
    INDENT(indent_level+1)
  );
  if(wssl->epoll_descriptor != WSSL_NO_DESCRIPTOR)
    fprintf(file, "%d", wssl->epoll_descriptor);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "global_extra_data: "
    ),
    INDENT(indent_level+1)
  );
  if(wssl->global_extra_data != WSSL_NULL)
    fprintf(file, "%p", wssl->global_extra_data);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "connect_callback_function: "
    ),
    INDENT(indent_level+1)
  );
  if(wssl->connect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    fprintf(file, "%p", wssl->connect_callback_function);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "disconnect_callback_function: "
    ),
    INDENT(indent_level+1)
  );
  if(wssl->disconnect_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    fprintf(file, "%p", wssl->disconnect_callback_function);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "receiving_callback_function: "
    ),
    INDENT(indent_level+1)
  );
  if(wssl->receiving_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    fprintf(file, "%p", wssl->receiving_callback_function);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "tick_callback_function: "
    ),
    INDENT(indent_level+1)
  );
  if(wssl->tick_callback_function != WSSL_CALLBACK_FUNCTION_NONE)
    fprintf(file, "%p", wssl->tick_callback_function);
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "next_client_id: "
    ),
    INDENT(indent_level+1)
  );
  wssl_id_dump(&wssl->next_client_id, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "servers:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_chain_is_not_empty(&wssl->servers))
  {
    wssl_chain_t* server_link;
    wssl_server_t* server;

    fprintf(file, "\n");
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
    {
      server = (wssl_server_t*)server_link;
      wssl_server_dump(server, file, indent_level+2);
    }
  }
  else
    fprintf(file, " none\n");
}
