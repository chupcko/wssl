#ifndef _RESULT_G_H_
#define _RESULT_G_H_

#define WSSL_RESULT_CODE_TABLE(what_call)                       \
  what_call(OK,                 0,   "OK"                     ) \
  what_call(ERROR_ERRNO,       -101, "System error with errno") \
  what_call(ERROR_MEMORY,      -102, "No memory"              ) \
  what_call(ERROR_FULL,        -103, "Buffer is full"         ) \
  what_call(ERROR_CONSISTENCY, -104, "Consistency error"      )/*#*/ \

_INCLUDE_BEGIN_

typedef enum wssl_result_code_e
{
  #define CALL(what_name, what_number, what_string) WSSL_RESULT_CODE_##what_name = what_number,
  WSSL_RESULT_CODE_TABLE(CALL)
  #undef CALL
  WSSL_RESULT_CODE_END_
} wssl_result_code_e;

_INCLUDE_END_

#endif
