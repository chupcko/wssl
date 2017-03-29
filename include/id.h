#ifndef _ID_H_
#define _ID_H_

_INCLUDE_BEGIN_

typedef uint32_t wssl_id_prefix_t;
#define WSSL_PRINT_ID_PREFIX PRIu32

typedef uint32_t wssl_id_suffix_t;
#define WSSL_PRINT_ID_SUFFIX PRIu32

typedef struct wssl_id_t
{
  wssl_id_prefix_t prefix;
  wssl_id_suffix_t suffix;
} wssl_id_t;

#define WSSL_ID_INIT_VALUE \
{                          \
  .prefix = 0,             \
  .suffix = 0              \
}                          \

static inline
void wssl_id_init
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->prefix = 0;
  id->suffix = 0;
}

static inline
void wssl_id_set_prefix
(
  _WSSL_MODIFY_       wssl_id_t*       id,
  _WSSL_IN_     const wssl_id_prefix_t prefix
)
{
  id->prefix = prefix;
}

static inline
void wssl_id_set_suffix
(
  _WSSL_MODIFY_       wssl_id_t*       id,
  _WSSL_IN_     const wssl_id_suffix_t suffix
)
{
  id->suffix = suffix;
}

static inline
bool wssl_id_is_equal
(
  _WSSL_IN_ const wssl_id_t* id_1,
  _WSSL_IN_ const wssl_id_t* id_2
)
{
  return
    id_1->prefix == id_2->prefix &&
    id_1->suffix == id_2->suffix;
}

static inline
bool wssl_id_is_not_equal
(
  _WSSL_IN_ const wssl_id_t* id_1,
  _WSSL_IN_ const wssl_id_t* id_2
)
{
  return
    id_1->prefix != id_2->prefix ||
    id_1->suffix != id_2->suffix;
}

static inline
void wssl_id_dump
(
  _WSSL_IN_ const wssl_id_t* id,
  _WSSL_IN_       FILE*      file
)
{
  fprintf
  (
    file,
    "%" WSSL_PRINT_ID_PREFIX ":%" WSSL_PRINT_ID_SUFFIX,
    id->prefix,
    id->suffix
  );
}

_INCLUDE_END_

static inline
void wssl_id_new
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->prefix++;
}

static inline
void wssl_id_next
(
  _WSSL_MODIFY_ wssl_id_t* id
)
{
  id->suffix++;
}

#endif
