#ifndef _LOGGING_G_H_
#define _LOGGING_G_H_

#define LOGGING_LEVEL_TABLE(what_call) \
  what_call(EMERGENCY, 0, "EMERGENCY") \
  what_call(ALERT,     1, "ALERT"    ) \
  what_call(CRITICAL,  2, "CRITICAL" ) \
  what_call(ERROR,     3, "ERROR"    ) \
  what_call(WARNING,   4, "WARNING"  ) \
  what_call(NOTICE,    5, "NOTICE"   ) \
  what_call(INFO,      6, "INFO"     ) \
  what_call(DEBUG,     7, "DEBUG"    ) \

_INCLUDE_BEGIN_

typedef enum wssl_logging_level_e
{
  #define MACRO_CALL(what_id, what_number, what_name) WSSL_LOGGING_LEVEL_##what_id = what_number,
  LOGGING_LEVEL_TABLE(MACRO_CALL)
  #undef MACRO_CALL
  LOGGING_LEVEL_TABLE_END_
} wssl_logging_level_e;

_INCLUDE_END_

#endif
