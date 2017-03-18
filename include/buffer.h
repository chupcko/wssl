#ifndef _BUFFER_H_
#define _BUFFER_H_

_INCLUDE_BEGIN_

typedef struct wssl_buffer_t
{
  wssl_octet_t* data;
  wssl_size_t   size;
  wssl_size_t   used;
} wssl_buffer_t;

_INCLUDE_END_

static inline void wssl_buffer_init
(
  _WSSL_MODIFY_ wssl_buffer_t* buffer
)
{
  buffer->data = WSSL_NULL;
  buffer->size = 0;
  buffer->used = 0;
}

static inline wssl_result_t wssl_buffer_create
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const wssl_size_t    size
)
{
  buffer->data = (wssl_octet_t*)malloc((size_t)size);
  if(buffer->data == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "buffer", 0);
  buffer->size = size;
  buffer->used = 0;
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}

static inline bool wssl_buffer_is_created
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->data != WSSL_NULL;
}

static inline bool wssl_buffer_is_not_created
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->data == WSSL_NULL;
}

static inline void wssl_buffer_clean
(
  _WSSL_MODIFY_ wssl_buffer_t* buffer
)
{
  free((void*)buffer->data);
  buffer->data = WSSL_NULL;
  buffer->size = 0;
  buffer->used = 0;
}

static inline wssl_result_t wssl_buffer_append
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size
)
{
  if(buffer->used+data_size > buffer->size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "buffer", 0);
  memcpy
  (
    (void*)&buffer->data[buffer->used],
    (void*)data,
    (size_t)data_size
  );
  buffer->used += data_size;
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);
}

static inline void wssl_buffer_shift
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const wssl_size_t    processed
)
{
  buffer->used -= processed;
  memmove
  (
    (void*)buffer->data,
    (void*)&buffer->data[processed],
    (size_t)buffer->used
  );
}

#endif