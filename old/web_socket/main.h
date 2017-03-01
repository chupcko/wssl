#ifndef _MAIN_H_
#define _MAIN_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#undef _EXTERN_
#undef _INIT_
#ifdef _MAIN_C_
  #define _EXTERN_
  #define _INIT_(...) = __VA_ARGS__
#else
  #define _EXTERN_ extern
  #define _INIT_(...)
#endif

#include "configuration.h"
#include "parameters.h"
#include "types.h"
#include "extra.h"
#include "variables.h"
#include "functions.h"
#include "prototypes.h"

#undef debug
#ifdef DEBUG
  #define debug(...) debug_real(__VA_ARGS__)
#else
  #define debug(...)
#endif

#define loop for(;;)

#endif
