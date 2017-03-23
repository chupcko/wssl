#ifndef _CHAIN_H_
#define _CHAIN_H_

_INCLUDE_BEGIN_

#define WSSL_CHAIN_INIT_VALUE(what_name) \
{                                        \
  .prev = &(what_name),                  \
  .next = &(what_name)                   \
}                                        \

_INCLUDE_END_

#define WSSL_CHAIN_FOR_EACH_LINK_FORWARD(what_link, what_chain) for \
(                                                                   \
  what_link = (what_chain)->next;                                   \
  what_link != (what_chain);                                        \
  what_link = what_link->next                                       \
)                                                                   \

#define WSSL_CHAIN_FOR_EACH_LINK_BACKWARD(what_link, what_chain) for \
(                                                                    \
  what_link = (what_chain)->prev;                                    \
  what_link != (what_chain);                                         \
  what_link = what_link->prev                                        \
)                                                                    \

#define WSSL_CHAIN_FOR_EACH_LINK_SAFE_FORWARD(what_link, what_link_next, what_chain) for \
(                                                                                        \
  what_link = (what_chain)->next,                                                        \
  what_link_next = what_link->next;                                                      \
  what_link != (what_chain);                                                             \
  what_link = what_link_next,                                                            \
  what_link_next = what_link->next                                                       \
)                                                                                        \

#define WSSL_CHAIN_FOR_EACH_LINK_SAFE_BACKWARD(what_link, what_link_prev, what_chain) for \
(                                                                                         \
  what_link = (what_chain)->prev,                                                         \
  what_link_prev = what_link->prev;                                                       \
  what_link != (what_chain);                                                              \
  what_link = what_link_prev,                                                             \
  what_link_prev = what_link->prev                                                        \
)                                                                                         \

#endif
