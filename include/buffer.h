#ifndef _BUFFER_H_
#define _BUFFER_H_

_INCLUDE_BEGIN_

typedef struct wssl_buffer_t
{
  wssl_octet_t* data;
  wssl_size_t   size;
  wssl_size_t   begin;
  wssl_size_t   end;
} wssl_buffer_t;

_INCLUDE_END_

static inline
void wssl_buffer_init
(
  _WSSL_MODIFY_ wssl_buffer_t* buffer
)
{
  buffer->data = WSSL_NULL;
  buffer->size = 0;
  buffer->begin = 0;
  buffer->end = 0;
}

static inline
wssl_result_t wssl_buffer_allocate
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const wssl_size_t    size
)
{
  buffer->data = (wssl_octet_t*)malloc((size_t)size);
  if(buffer->data == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "buffer", 0);
  buffer->size = size;
  buffer->begin = 0;
  buffer->end = 0;
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline
bool wssl_buffer_is_allocated
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->data != WSSL_NULL;
}

static inline
bool wssl_buffer_is_not_allocated
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->data == WSSL_NULL;
}

static inline
bool wssl_buffer_is_empty
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->begin >= buffer->end;
}

static inline
bool wssl_buffer_is_not_empty
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->begin < buffer->end;
}

static inline
bool wssl_buffer_is_full
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->end >= buffer->size;
}

static inline
bool wssl_buffer_is_not_full
(
  _WSSL_IN_ const wssl_buffer_t* buffer
)
{
  return buffer->end < buffer->size;
}

static inline
void wssl_buffer_free
(
  _WSSL_MODIFY_ wssl_buffer_t* buffer
)
{
  free((void*)buffer->data);
  buffer->data = WSSL_NULL;
  buffer->size = 0;
  buffer->begin = 0;
  buffer->end = 0;
}

static inline
void wssl_buffer_printf
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const char*          format,
                                     ...
)
{
  va_list arguments;
  va_start(arguments, format);
  buffer->end = vsnprintf
  (
    (char*)&buffer->data[buffer->begin],
    (size_t)buffer->size,
    format,
    arguments
  );
  va_end(arguments);
}

static inline
wssl_result_t wssl_buffer_append
(
  _WSSL_MODIFY_       wssl_buffer_t* buffer,
  _WSSL_IN_     const wssl_octet_t*  data,
  _WSSL_IN_     const wssl_size_t    data_size
)
{
  if(buffer->end+data_size > buffer->size)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_FULL, "buffer", 0);
  memcpy
  (
    (void*)&buffer->data[buffer->end],
    (void*)data,
    (size_t)data_size
  );
  buffer->end += data_size;
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline
void wssl_buffer_shift_left
(
  _WSSL_MODIFY_ wssl_buffer_t* buffer
)
{
  if(buffer->begin > 0)
  {
    buffer->end -= buffer->begin;
    memmove
    (
      (void*)buffer->data,
      (void*)&buffer->data[buffer->begin],
      (size_t)buffer->end
    );
    buffer->begin = 0;
  }
}

#endif
