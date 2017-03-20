#include "main.h"

#define WSSL_DUMP_POINTER(what_name, what_null) \
fprintf                                         \
(                                               \
  file,                                         \
  (                                             \
    "\n"                                        \
    INDENT_FORMAT #what_name ": "               \
  ),                                            \
  INDENT(indent_level+1)                        \
);                                              \
if(wssl->what_name != what_null)                \
  fprintf(file, "%p", wssl->what_name);         \
else                                            \
  fprintf(file, "none");                        \

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
  WSSL_DUMP_POINTER(global_extra_data,             WSSL_NULL)
  WSSL_DUMP_POINTER(connect_callback,              WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(disconnect_callback,           WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(header_callback,               WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_text_frame_callback,   WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_binary_frame_callback, WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_close_frame_callback,  WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_ping_callback,         WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_pong_callback,         WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(tick_callback,                 WSSL_CALLBACK_NONE)
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "random_seed: %u\n"
      INDENT_FORMAT "next_client_id: "
    ),
    INDENT(indent_level+1), wssl->random_seed,
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

    fprintf(file, "\n");
    WSSL_CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
      wssl_server_dump((wssl_server_t*)server_link, file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}