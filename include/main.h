#ifndef _MAIN_H_
#define _MAIN_H_

#undef _FUNCTION_
#define _FUNCTION_
#undef _LIBRARY_FUNCTION_
#define _LIBRARY_FUNCTION_
#undef _INCLUDE_BEGIN_
#define _INCLUDE_BEGIN_
#undef _INCLUDE_END_
#define _INCLUDE_END_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

_INCLUDE_BEGIN_

#include <sys/epoll.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#undef _WSSL_IN_
#define _WSSL_IN_
#undef _WSSL_OUT_
#define _WSSL_OUT_
#undef _WSSL_MODIFY_
#define _WSSL_MODIFY_

_INCLUDE_END_

#include "version.h"
#include "configurations.h"
#include "base.h"
#include "chain.h"
#include "result.h"
#include "epoll_data.h"
#include "server.h"
#include "id.h"
#include "buffer.h"
#include "header.h"
#include "client.h"
#include "callback.h"
#include "wssl.h"
#include "inline.h"

#include "prototype.h"

#define LOOP for(;;)

#endif
