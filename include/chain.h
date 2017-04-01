#ifndef _CHAIN_H_
#define _CHAIN_H_

_INCLUDE_BEGIN_

#define WSSL_CHAIN_INIT_VALUE(what_name) \
{                                        \
  .prev = &(what_name),                  \
  .next = &(what_name)                   \
}                                        \

_INCLUDE_END_

#define MAKE_CHAIN_ENTRY(what_prefix, what_type_chain, what_type_entry, what_entry_member) \
                                                                                           \
static inline                                                                              \
what_type_entry* what_prefix##_chain_get_entry_from_##what_entry_member                    \
(                                                                                          \
  _WSSL_IN_ const what_type_chain* link                                                    \
)                                                                                          \
{                                                                                          \
  what_type_entry* entry = (what_type_entry*)                                              \
  (                                                                                        \
    (void*)link-                                                                           \
    (size_t)&((what_type_entry*)0)->what_entry_member                                      \
  );                                                                                       \
  WSSL_ASSERT                                                                              \
  (                                                                                        \
    &entry->what_entry_member == link &&                                                   \
    what_prefix##_chain_is_equal(&entry->what_entry_member, link)                          \
  );                                                                                       \
  return entry;                                                                            \
}                                                                                          \

#define CHAIN_FOR_EACH_LINK_FORWARD(what_link, what_chain) for \
(                                                              \
  what_link = (what_chain)->next;                              \
  what_link != (what_chain);                                   \
  what_link = what_link->next                                  \
)                                                              \

#define CHAIN_FOR_EACH_LINK_BACKWARD(what_link, what_chain) for \
(                                                               \
  what_link = (what_chain)->prev;                               \
  what_link != (what_chain);                                    \
  what_link = what_link->prev                                   \
)                                                               \

#define CHAIN_FOR_EACH_LINK_SAFE_FORWARD(what_link, what_link_next, what_chain) for \
(                                                                                   \
  what_link = (what_chain)->next,                                                   \
  what_link_next = what_link->next;                                                 \
  what_link != (what_chain);                                                        \
  what_link = what_link_next,                                                       \
  what_link_next = what_link->next                                                  \
)                                                                                   \

#define CHAIN_FOR_EACH_LINK_SAFE_BACKWARD(what_link, what_link_prev, what_chain) for \
(                                                                                    \
  what_link = (what_chain)->prev,                                                    \
  what_link_prev = what_link->prev;                                                  \
  what_link != (what_chain);                                                         \
  what_link = what_link_prev,                                                        \
  what_link_prev = what_link->prev                                                   \
)                                                                                    \

#endif
