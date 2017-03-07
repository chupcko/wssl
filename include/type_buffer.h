#ifndef _TYPE_BUFFER_H_
#define _TYPE_BUFFER_H_

_INCLUDE_BEGIN_

typedef struct wssl_buffer_t
{
  wssl_octet_t* data;
  wssl_size_t   size;
  wssl_size_t   used;
} wssl_buffer_t;

_INCLUDE_END_

#define WSSL_BUFFER_INIT(what_buffer) \
do                                    \
{                                     \
  (what_buffer).data = WSSL_NULL;     \
  (what_buffer).size = 0;             \
  (what_buffer).used = 0;             \
}                                     \
while(false)                          \

#define WSSL_BUFFER_IS_SET(what_buffer) ((what_buffer).data != WSSL_NULL)

#define WSSL_BUFFER_IS_NOT_SET(what_buffer) ((what_buffer).data == WSSL_NULL)

#define WSSL_BUFFER_CREATE(what_buffer, what_size, what_place)             \
do                                                                         \
{                                                                          \
  (what_buffer).data = (wssl_octet_t*)malloc((size_t)(what_size));         \
  if((what_buffer).data == NULL)                                           \
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, what_place, 0); \
  (what_buffer).size = (what_size);                                        \
  (what_buffer).used = 0;                                                  \
}                                                                          \
while(false)                                                               \

#define WSSL_BUFFER_DELETE(what_buffer) \
do                                      \
{                                       \
  free((void*)(what_buffer).data);      \
  (what_buffer).data = WSSL_NULL;       \
  (what_buffer).size = 0;               \
  (what_buffer).used = 0;               \
}                                       \
while(false)                            \

#define WSSL_BUFFER_MOVE(what_buffer, what_processed) \
do                                                    \
{                                                     \
  (what_buffer).used -= (what_processed);             \
  memmove                                             \
  (                                                   \
    (void*)(what_buffer).data,                        \
    (void*)&(what_buffer).data[(what_processed)],     \
    (what_buffer).used                                \
  );                                                  \
}                                                     \
while(false)

#endif
