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
  fprintf(file, "%p", (void*)wssl->what_name);  \
else                                            \
  fprintf(file, "none");                        \

_LIBRARY_FUNCTION_
void wssl_dump
(
  _WSSL_IN_     const wssl_t*     wssl,
  _WSSL_MODIFY_       FILE*       file,
  _WSSL_IN_     const wssl_size_t indent_level
)
{
  fprintf
  (
    file,
    (
      INDENT_FORMAT "wssl(%p)=\n"
      INDENT_FORMAT "epoll_descriptor: "
    ),
    INDENT(indent_level), (void*)wssl,
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
  WSSL_DUMP_POINTER(receive_ping_frame_callback,   WSSL_CALLBACK_NONE)
  WSSL_DUMP_POINTER(receive_pong_frame_callback,   WSSL_CALLBACK_NONE)
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
  wssl_id_print(&wssl->next_client_id, file);
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "servers:"
    ),
    INDENT(indent_level+1)
  );
  if(wssl_server_chain_is_not_empty(&wssl->servers))
  {
    fprintf(file, "\n");
    wssl_server_chain_t* server_link;
    CHAIN_FOR_EACH_LINK_FORWARD(server_link, &wssl->servers)
      wssl_server_dump(wssl_server_chain_get_entry_from_chain_link(server_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
  fprintf(file, INDENT_FORMAT "clients_for_disconnecting:", INDENT(indent_level+1));
  if(wssl_client_chain_is_not_empty(&wssl->clients_for_disconnecting))
  {
    fprintf(file, "\n");
    wssl_client_chain_t* client_link;
    CHAIN_FOR_EACH_LINK_FORWARD(client_link, &wssl->clients_for_disconnecting)
      wssl_client_dump(wssl_client_chain_get_entry_from_chain_link(client_link), file, indent_level+2);
  }
  else
    fprintf(file, " none\n");
}
