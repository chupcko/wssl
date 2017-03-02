#ifndef _CHAIN_H_
#define _CHAIN_H_

typedef struct wssl_chain_t
{
  struct wssl_chain_t* next;
  struct wssl_chain_t* prev;
} wssl_chain_t;

#define WSSL_CHAIN_INIT(name) { &(name), &(name) }

static inline void wssl_chain_init(wssl_chain_t* chain)
{
  chain->next = chain;
  chain->prev = chain;
}

static inline void wssl_chain_add_link_forward(wssl_chain_t* link, wssl_chain_t* new)
{
  new->next = link->next;
  new->prev = link;
  link->next->prev = new;
  link->next = new;
}

static inline void wssl_chain_add_link_backward(wssl_chain_t* link, wssl_chain_t* new)
{
  new->next = link;
  new->prev = link->prev;
  link->prev->next = new;
  link->prev = new;
}

static inline bool wssl_chain_is_empty(wssl_chain_t* chain)
{
  return
    chain == chain->next ||
    chain == chain->prev;
}

static inline void wssl_chain_delete_link(wssl_chain_t* link)
{
  if(wssl_chain_is_empty(link))
    return;
  link->next->prev = link->prev;
  link->prev->next = link->next;
  link->next = (wssl_chain_t*)NULL;
  link->prev = (wssl_chain_t*)NULL;
}

#define WSSL_CHAIN_FOR_EACH_LINK_FORWARD(link, link_next, chain) for \
(                                                                    \
  link = (chain)->next,                                              \
  link_next = link->next;                                            \
  link != (chain);                                                   \
  link = link_next,                                                  \
  link_next = link->next                                             \
)                                                                    \

#define WSSL_CHAIN_FOR_EACH_LINK_BACKWARD(link, link_prev, chain) for \
(                                                                     \
  link = (chain)->prev,                                               \
  link_prev = link->prev;                                             \
  link != (chain);                                                    \
  link = link_prev,                                                   \
  link_prev = link->prev                                              \
)                                                                     \

#endif
