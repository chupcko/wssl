#ifndef _DEBUG_H_
#define _DEBUG_H_

_INCLUDE_BEGIN_

#ifdef WSSL_DEBUG

static inline
void wssl_assert_fail
(
  const char*        expression,
  const char*        file_name,
  const unsigned int line_number,
  const char*        function_name
)
{
  fprintf
  (
    stderr,
    "WSSL %s:%u %s: assertion \'%s\' failed\n",
    file_name,
    line_number,
    function_name,
    expression
  );
  abort();
}

#define WSSL_ASSERT(what_expression) \
(                                    \
  (what_expression) ?                \
  (void)(0) :                        \
  wssl_assert_fail                   \
  (                                  \
    #what_expression,                \
    __FILE__,                        \
    __LINE__,                        \
    __func__                         \
  )                                  \
)                                    \

#else

#define WSSL_ASSERT(what_expression) ((void)0)

#endif

_INCLUDE_END_

#endif
