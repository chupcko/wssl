#ifndef _RESULT_H_
#define _RESULT_H_

#define WSSL_RESULT_CODE_TABLE(what_call)                       \
  what_call(OK,                 0,   "OK"                     ) \
  what_call(ERROR_ERRNO,       -101, "System error with errno") \
  what_call(ERROR_MEMORY,      -102, "No memory"              ) \
  what_call(ERROR_FULL,        -103, "Buffer is full"         ) \
  what_call(ERROR_CONSISTENCY, -104, "Consistency error"      ) \

_INCLUDE_BEGIN_

typedef enum wssl_result_code_e
{
  #define CALL(what_name, what_number, what_string) WSSL_RESULT_CODE_##what_name = what_number,
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  WSSL_RESULT_CODE_END_
} wssl_result_code_e;

typedef struct wssl_result_t
{
  wssl_result_code_e code;
  char*              where;
  int                last_errno;
} wssl_result_t;

_INCLUDE_END_

#define WSSL_MAKE_RESULT(what_code, what_where, what_last_errno) (wssl_result_t) \
{                                                                                \
  .code       = (what_code),                                                     \
  .where      = (what_where),                                                    \
  .last_errno = (what_last_errno)                                                \
}                                                                                \

#define WSSL_CALL(...)                     \
do                                         \
{                                          \
  wssl_result_t _result_ = (__VA_ARGS__);  \
                                           \
  if(_result_.code != WSSL_RESULT_CODE_OK) \
    return _result_;                       \
}                                          \
while(false)                               \

#endif
