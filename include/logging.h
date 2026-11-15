#ifndef _LOGGING_H_
#define _LOGGING_H_

_INCLUDE_BEGIN_

typedef struct wssl_logging_t
{
  int   descriptor;
  char* ident;
  char  hostname[WSSL_HOSTNAME_SIZE_IN_CHAR];
} wssl_logging_t;

#define WSSL_LOGGING_INIT_VALUE     \
{                                   \
  .descriptor = WSSL_NO_DESCRIPTOR, \
  .ident      = WSSL_NULL,          \
  .hostname   = ""                  \
}                                   \

_INCLUDE_END_

#endif
