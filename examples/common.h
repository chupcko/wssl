#ifndef _COMMON_H_
#define _COMMON_H_

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "wssl.h"

#define ERROR(format, ...)                \
do                                        \
{                                         \
  fprintf(stderr, format, ##__VA_ARGS__); \
  exit(EXIT_FAILURE);                     \
}                                         \
while(false)                              \

#define CALL(...)                         \
do                                        \
{                                         \
  wssl_result_t _result_ = (__VA_ARGS__); \
  if(wssl_result_is_not_ok(_result_))     \
  {                                       \
    wssl_result_print(_result_, stderr);  \
    exit(EXIT_FAILURE);                   \
  }                                       \
}                                         \
while(false)                              \

#endif
