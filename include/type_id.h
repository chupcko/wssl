#ifndef _TYPE_ID_H_
#define _TYPE_ID_H_

_INCLUDE_BEGIN_

typedef struct wssl_id_t
{
  uint16_t prefix;
  uint32_t suffix;
} wssl_id_t;

#define WSSL_ID_INIT_VALUE \
{                          \
  .prefix = 0,             \
  .suffix = 0              \
}                          \

static inline void wssl_id_init
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->prefix = 0;
  id->suffix = 0;
}

static inline bool wssl_id_is_equal
(
  _WSSL_IN_ const wssl_id_t* id_1,
  _WSSL_IN_ const wssl_id_t* id_2
)
{
  return
    id_1->prefix == id_2->prefix &&
    id_1->suffix == id_2->suffix;
}

static inline bool wssl_id_is_not_equal
(
  _WSSL_IN_ const wssl_id_t* id_1,
  _WSSL_IN_ const wssl_id_t* id_2
)
{
  return
    id_1->prefix != id_2->prefix ||
    id_1->suffix != id_2->suffix;
}

_INCLUDE_END_

static inline void wssl_id_new
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->prefix++;
}

static inline void wssl_id_next
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->suffix++;
}

#endif
