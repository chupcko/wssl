#ifndef _RESULT_G_H_
#define _RESULT_G_H_

#define RESULT_CODE_TABLE(what_call)                           \
  what_call(OK,                0,   "OK"                     ) \
  what_call(ERROR_ERRNO,      -101, "System error with errno") \
  what_call(ERROR_NO_MEMORY,  -102, "No memory"              ) \
  what_call(ERROR_NO_SPACE,   -103, "No space"               ) \
  what_call(ERROR_BAD_SYNTAX, -104, "Bad syntax"             ) \

_INCLUDE_BEGIN_

typedef enum wssl_result_code_e
{
  #define MACRO_CALL(what_id, what_number, what_name) WSSL_RESULT_CODE_##what_id = what_number,
  RESULT_CODE_TABLE(MACRO_CALL)
  #undef MACRO_CALL
  WSSL_RESULT_CODE_END_
} wssl_result_code_e;

_INCLUDE_END_

#endif
