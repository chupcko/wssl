#ifndef _CHAIN_G_H_
#define _CHAIN_G_H_

#define MAKE_CHAIN_INCLUDE(what_prefix, what_type_chain) \
                                                         \
typedef struct what_type_chain                           \
{                                                        \
  struct what_type_chain* prev;                          \
  struct what_type_chain* next;                          \
} what_type_chain;                                       \
                                                         \
static inline                                            \
void what_prefix##_chain_root_init                       \
(                                                        \
  _WSSL_MODIFY_ what_type_chain* root                    \
)                                                        \
{                                                        \
  root->prev = root;                                     \
  root->next = root;                                     \
}                                                        \
                                                         \
static inline                                            \
void what_prefix##_chain_link_init                       \
(                                                        \
  _WSSL_MODIFY_ what_type_chain* link                    \
)                                                        \
{                                                        \
  link->prev = WSSL_NULL;                                \
  link->next = WSSL_NULL;                                \
}                                                        \

#define MAKE_CHAIN(what_prefix, what_type_chain)       \
                                                       \
static inline                                          \
void what_prefix##_chain_add_link_forward              \
(                                                      \
  _WSSL_MODIFY_ what_type_chain* chain,                \
  _WSSL_MODIFY_ what_type_chain* new_link              \
)                                                      \
{                                                      \
  new_link->prev = chain;                              \
  new_link->next = chain->next;                        \
  chain->next->prev = new_link;                        \
  chain->next = new_link;                              \
}                                                      \
                                                       \
static inline                                          \
void what_prefix##_chain_add_link_backward             \
(                                                      \
  _WSSL_MODIFY_ what_type_chain* chain,                \
  _WSSL_MODIFY_ what_type_chain* new_link              \
)                                                      \
{                                                      \
  new_link->prev = chain->prev;                        \
  new_link->next = chain;                              \
  chain->prev->next = new_link;                        \
  chain->prev = new_link;                              \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_empty                      \
(                                                      \
  _WSSL_IN_ const what_type_chain* root                \
)                                                      \
{                                                      \
  return                                               \
    root == root->prev ||                              \
    root == root->next;                                \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_not_empty                  \
(                                                      \
  _WSSL_IN_ const what_type_chain* root                \
)                                                      \
{                                                      \
  return                                               \
    root != root->prev &&                              \
    root != root->next;                                \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_null                       \
(                                                      \
  _WSSL_IN_ const what_type_chain* link                \
)                                                      \
{                                                      \
  return                                               \
    link->prev == WSSL_NULL ||                         \
    link->next == WSSL_NULL;                           \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_not_null                   \
(                                                      \
  _WSSL_IN_ const what_type_chain* link                \
)                                                      \
{                                                      \
  return                                               \
    link->prev != WSSL_NULL &&                         \
    link->next != WSSL_NULL;                           \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_equal                      \
(                                                      \
  _WSSL_IN_ const what_type_chain* chain_1,            \
  _WSSL_IN_ const what_type_chain* chain_2             \
)                                                      \
{                                                      \
  return                                               \
    chain_1->prev == chain_2->prev &&                  \
    chain_1->next == chain_2->next;                    \
}                                                      \
                                                       \
static inline                                          \
bool what_prefix##_chain_is_not_equal                  \
(                                                      \
  _WSSL_IN_ const what_type_chain* chain_1,            \
  _WSSL_IN_ const what_type_chain* chain_2             \
)                                                      \
{                                                      \
  return                                               \
    chain_1->prev != chain_2->prev ||                  \
    chain_1->next != chain_2->next;                    \
}                                                      \
                                                       \
static inline                                          \
void what_prefix##_chain_delete_link                   \
(                                                      \
  _WSSL_MODIFY_ what_type_chain* link                  \
)                                                      \
{                                                      \
  WSSL_ASSERT(what_prefix##_chain_is_not_empty(link)); \
  if(what_prefix##_chain_is_not_null(link))            \
  {                                                    \
    link->prev->next = link->next;                     \
    link->next->prev = link->prev;                     \
    link->prev = WSSL_NULL;                            \
    link->next = WSSL_NULL;                            \
  }                                                    \
}                                                      \

_INCLUDE_BEGIN_

MAKE_CHAIN_INCLUDE(wssl_server,       wssl_server_chain_t      )
MAKE_CHAIN_INCLUDE(wssl_chunk,        wssl_chunk_chain_t       )
MAKE_CHAIN_INCLUDE(wssl_header_field, wssl_header_field_chain_t)
MAKE_CHAIN_INCLUDE(wssl_client,       wssl_client_chain_t      )

_INCLUDE_END_

MAKE_CHAIN(wssl_server,       wssl_server_chain_t      )
MAKE_CHAIN(wssl_chunk,        wssl_chunk_chain_t       )
MAKE_CHAIN(wssl_header_field, wssl_header_field_chain_t)
MAKE_CHAIN(wssl_client,       wssl_client_chain_t      )

#endif
