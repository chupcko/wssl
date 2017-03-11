#ifndef _CHAIN_H_
#define _CHAIN_H_

_INCLUDE_BEGIN_

typedef struct wssl_chain_t
{
  struct wssl_chain_t* next;
  struct wssl_chain_t* prev;
} wssl_chain_t;

#define WSSL_CHAIN_INIT_VALUE(what_name) \
{                                        \
  .next = &(what_name),                  \
  .prev = &(what_name)                   \
}                                        \

#define WSSL_CHAIN_DECLARE(what_name) wssl_chain_t what_name = WSSL_CHAIN_INIT_VALUE(what_name)

static inline void wssl_chain_init
(
  wssl_chain_t* chain
)
{
  chain->next = chain;
  chain->prev = chain;
}

_INCLUDE_END_

static inline void wssl_chain_add_link_forward
(
  _WSSL_MODIFY_ wssl_chain_t* link,
  _WSSL_MODIFY_ wssl_chain_t* new
)
{
  new->next = link->next;
  new->prev = link;
  link->next->prev = new;
  link->next = new;
}

static inline void wssl_chain_add_link_backward
(
  _WSSL_MODIFY_ wssl_chain_t* link,
  _WSSL_MODIFY_ wssl_chain_t* new
)
{
  new->next = link;
  new->prev = link->prev;
  link->prev->next = new;
  link->prev = new;
}

static inline bool wssl_chain_is_empty
(
  _WSSL_IN_ const wssl_chain_t* chain
)
{
  return
    chain == chain->next ||
    chain == chain->prev;
}

static inline void wssl_chain_delete_link
(
  _WSSL_MODIFY_ wssl_chain_t* link
)
{
  if(wssl_chain_is_empty(link))
    return;
  link->next->prev = link->prev;
  link->prev->next = link->next;
  link->next = (wssl_chain_t*)WSSL_NULL;
  link->prev = (wssl_chain_t*)WSSL_NULL;
}

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
