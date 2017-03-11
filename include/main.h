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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

_INCLUDE_BEGIN_

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
#include "type_epoll.h"
#include "type_server.h"
#include "type_id.h"
#include "type_buffer.h"
#include "type_header.h"
#include "type_client.h"
#include "type_callback.h"
#include "type_wssl.h"

#include "prototype.h"

#define LOOP for(;;)

#endif
