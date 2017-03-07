#ifndef _TYPE_ID_H_
#define _TYPE_ID_H_

_INCLUDE_BEGIN_

typedef struct wssl_id_t
{
  uint16_t prefix;
  uint32_t suffix;
} wssl_id_t;

#define WSSL_ID_IS_EQUAL(what_id_1, what_id_2) ((what_id_1).prefix == (what_id_2).prefix && (what_id_1).suffix == (what_id_2).suffix)

#define WSSL_ID_IS_NOT_EQUAL(what_id_1, what_id_2) ((what_id_1).prefix != (what_id_2).prefix || (what_id_1).suffix != (what_id_2).suffix)

#define WSSL_ID_INIT_VALUE \
{                          \
  .prefix = 0,             \
  .suffix = 0              \
}                          \

static inline void wssl_id_init(wssl_id_t* id)
{
  id->prefix = 0;
  id->suffix = 0;
}

_INCLUDE_END_

#define WSSL_ID_NEW(what_id) ((what_id).prefix++)

#define WSSL_ID_NEXT(what_id) ((what_id).suffix++)

#endif
