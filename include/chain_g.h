#ifndef _CHAIN_G_H_
#define _CHAIN_G_H_

#define MAKE_CHAIN_INCLUDE(what_prefix, what_type_chain, what_type_entry) \
                                                                          \
typedef struct what_type_chain                                            \
{                                                                         \
  struct what_type_chain* prev;                                           \
  struct what_type_chain* next;                                           \
} what_type_chain;                                                        \
                                                                          \
static inline                                                             \
void what_prefix##_chain_init                                             \
(                                                                         \
  _WSSL_MODIFY_ what_type_chain* chain                                    \
)                                                                         \
{                                                                         \
  chain->prev = chain;                                                    \
  chain->next = chain;                                                    \
}                                                                         \

#define MAKE_CHAIN(what_prefix, what_type_chain, what_type_entry) \
                                                                  \
static inline                                                     \
void what_prefix##_chain_add_link_forward                         \
(                                                                 \
  _WSSL_MODIFY_ what_type_chain* link,                            \
  _WSSL_MODIFY_ what_type_chain* new                              \
)                                                                 \
{                                                                 \
  new->prev = link;                                               \
  new->next = link->next;                                         \
  link->next->prev = new;                                         \
  link->next = new;                                               \
}                                                                 \
                                                                  \
static inline                                                     \
void what_prefix##_chain_add_link_backward                        \
(                                                                 \
  _WSSL_MODIFY_ what_type_chain* link,                            \
  _WSSL_MODIFY_ what_type_chain* new                              \
)                                                                 \
{                                                                 \
  new->prev = link->prev;                                         \
  new->next = link;                                               \
  link->prev->next = new;                                         \
  link->prev = new;                                               \
}                                                                 \
                                                                  \
static inline                                                     \
bool what_prefix##_chain_is_empty                                 \
(                                                                 \
  _WSSL_IN_ const what_type_chain* chain                          \
)                                                                 \
{                                                                 \
  return                                                          \
    chain == chain->prev ||                                       \
    chain == chain->next;                                         \
}                                                                 \
                                                                  \
static inline                                                     \
bool what_prefix##_chain_is_not_empty                             \
(                                                                 \
  _WSSL_IN_ const what_type_chain* chain                          \
)                                                                 \
{                                                                 \
  return                                                          \
    chain != chain->prev &&                                       \
    chain != chain->next;                                         \
}                                                                 \
                                                                  \
static inline                                                     \
void what_prefix##_chain_delete_link                              \
(                                                                 \
  _WSSL_MODIFY_ what_type_chain* link                             \
)                                                                 \
{                                                                 \
  if(what_prefix##_chain_is_not_empty(link))                      \
  {                                                               \
    link->prev->next = link->next;                                \
    link->next->prev = link->prev;                                \
    link->prev = WSSL_NULL;                                       \
    link->next = WSSL_NULL;                                       \
  }                                                               \
}                                                                 \
                                                                  \
static inline                                                     \
what_type_entry* what_prefix##_chain_entry                        \
(                                                                 \
  _WSSL_IN_ what_type_chain* link                                 \
)                                                                 \
{                                                                 \
  return (what_type_entry*)link;                                  \
}                                                                 \

_INCLUDE_BEGIN_

MAKE_CHAIN_INCLUDE(wssl_client,       wssl_client_chain_t,       struct wssl_client_t      )
MAKE_CHAIN_INCLUDE(wssl_server,       wssl_server_chain_t,       struct wssl_server_t      )
MAKE_CHAIN_INCLUDE(wssl_chunk,        wssl_chunk_chain_t,        struct wssl_chunk_t       )
MAKE_CHAIN_INCLUDE(wssl_header_field, wssl_header_field_chain_t, struct wssl_header_field_t)

_INCLUDE_END_

#undef MAKE_CHAIN_INCLUDE

MAKE_CHAIN(wssl_client,       wssl_client_chain_t,       struct wssl_client_t      )
MAKE_CHAIN(wssl_server,       wssl_server_chain_t,       struct wssl_server_t      )
MAKE_CHAIN(wssl_chunk,        wssl_chunk_chain_t,        struct wssl_chunk_t       )
MAKE_CHAIN(wssl_header_field, wssl_header_field_chain_t, struct wssl_header_field_t)

#undef MAKE_CHAIN

#endif
