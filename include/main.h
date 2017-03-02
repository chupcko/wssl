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

_INCLUDE_BEGIN_

#undef _WSSL_IN_
#define _WSSL_IN_
#undef _WSSL_OUT_
#define _WSSL_OUT_
#undef _WSSL_MODIFY_
#define _WSSL_MODIFY_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

_INCLUDE_END_

#include "chain.h"
#include "types.h"
#include "version.h"

#define loop for(;;)

#endif
