#ifndef _RESULT_H_
#define _RESULT_H_

_INCLUDE_BEGIN_

typedef struct wssl_result_t
{
  wssl_result_code_e code;
  char*              where;
  union
  {
    int              system_errno;
    int              callback_error;
  };
} wssl_result_t;

static inline
bool wssl_result_is_ok
(
  wssl_result_t result
)
{
  return result.code == WSSL_RESULT_CODE_OK;
}

static inline
bool wssl_result_is_not_ok
(
  wssl_result_t result
)
{
  return result.code != WSSL_RESULT_CODE_OK;
}

_INCLUDE_END_

#define MAKE_RESULT_OK                   \
(                                        \
  (wssl_result_t)                        \
  {                                      \
    .code         = WSSL_RESULT_CODE_OK, \
    .where        = WSSL_NULL            \
  }                                      \
)                                        \

#define MAKE_RESULT_ERRNO(what_where, what_system_errno) \
(                                                        \
  (wssl_result_t)                                        \
  {                                                      \
    .code         = WSSL_RESULT_CODE_ERROR_ERRNO,        \
    .where        = (what_where),                        \
    .system_errno = (what_system_errno)                  \
  }                                                      \
)                                                        \

#define MAKE_RESULT(what_code, what_where) \
(                                          \
  (wssl_result_t)                          \
  {                                        \
    .code         = (what_code),           \
    .where        = (what_where)           \
  }                                        \
)                                          \

#define PASS                                      return MAKE_RESULT_OK
#define FAIL_ERRNO(what_where, what_system_errno) return MAKE_RESULT_ERRNO(what_where, what_system_errno)
#define FAIL_ERROR(what_where, what_code)         return MAKE_RESULT(what_code, what_where)

#define TRY_CALL(...)                     \
do                                        \
{                                         \
  wssl_result_t _result_ = (__VA_ARGS__); \
  if(wssl_result_is_not_ok(_result_))     \
    return _result_;                      \
}                                         \
while(false)                              \

#endif
