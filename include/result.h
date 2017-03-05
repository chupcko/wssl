#ifndef _RESULT_H_
#define _RESULT_H_

#define WSSL_RESULT_CODE_TABLE(call)            \
  call(OK,      0,   "OK")                      \
  call(ERRNO,  -101, "System error with errno") \
  call(MEMORY, -102, "No memory")               \

_INCLUDE_BEGIN_

typedef enum wssl_result_code_t
{

  #define CALL(name, number, string) WSSL_RESULT_CODE_##name = number,
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  WSSL_RESULT_CODE_END_
} wssl_result_code_t;

typedef struct wssl_result_t
{
  wssl_result_code_t code;
  char*              where;
  int                last_errno;
} wssl_result_t;

_INCLUDE_END_

#define WSSL_MAKE_RESULT(what_code, what_where, what_last_errnoa) (wssl_result_t) \
{                                                                                 \
  .code       = (what_code),                                                      \
  .where      = (what_where),                                                     \
  .last_errno = (what_last_errnoa)                                                \
}                                                                                 \

#define WSSL_CALL(...)                     \
do                                         \
{                                          \
  wssl_result_t _result_;                  \
                                           \
  _result_ = (__VA_ARGS__);                \
  if(_result_.code != WSSL_RESULT_CODE_OK) \
    return _result_;                       \
}                                          \
while(0)                                   \

#endif
