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
  fprintf(stderr, "\n");                  \
  exit(EXIT_FAILURE);                     \
}                                         \
while(false)                              \

#endif
