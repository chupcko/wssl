#ifndef _MAIN_H_
#define _MAIN_H_

#define _FUNCTION_
#define _LIBRARY_FUNCTION_
#define _INCLUDE_BEGIN_
#define _INCLUDE_END_

#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

_INCLUDE_BEGIN_

#include <sys/epoll.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define _WSSL_IN_
#define _WSSL_OUT_
#define _WSSL_MODIFY_

_INCLUDE_END_

#define LOOP for(;;)

#include "version.h"
#include "configurations.h"
#include "debug.h"
#include "base.h"
#include "chain_g.h"
#include "chain.h"
#include "result_g.h"
#include "result.h"
#include "epoll_data.h"
#include "server.h"
#include "id.h"
#include "buffer.h"
#include "chunk.h"
#include "header.h"
#include "frame.h"
#include "client_g.h"
#include "client.h"
#include "callback.h"
#include "wssl.h"
#include "wssl_g.h"

#include "prototype.h"

#include "rest.h"

#endif
