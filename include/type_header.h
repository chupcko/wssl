#ifndef _TYPE_HEADER_H_
#define _TYPE_HEADER_H_

_INCLUDE_BEGIN_

typedef struct wssl_header_field_t
{
  wssl_chain_t chain_link;
  char*        key;
  char*        value;
} wssl_header_field_t;

typedef struct wssl_header_t
{
  char*        method;
  char*        uri;
  char*        version;
  wssl_chain_t fields;
} wssl_header_t;

_INCLUDE_END_

static inline void wssl_header_init
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  header->method = WSSL_NULL;
  header->uri = WSSL_NULL;
  header->version = WSSL_NULL;
  wssl_chain_init(&header->fields);
}

#define MAKE_HEADER_INSERT(what_member)                                                \
static inline wssl_result_t wssl_header_insert_##what_member                           \
(                                                                                      \
  _WSSL_MODIFY_ wssl_header_t* header,                                                 \
  _WSSL_IN_     char*          data,                                                   \
  _WSSL_IN_     wssl_size_t    data_size                                               \
)                                                                                      \
{                                                                                      \
  header->what_member = (char*)malloc((size_t)(data_size+1));                          \
  if(header->what_member == NULL)                                                      \
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header " #what_member, 0); \
  strncpy(header->what_member, data, data_size);                                       \
  header->what_member[data_size] = '\0';                                               \
                                                                                       \
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, NULL, 0);                               \
}                                                                                      \

MAKE_HEADER_INSERT(method)
MAKE_HEADER_INSERT(uri)
MAKE_HEADER_INSERT(version)
#undef MAKE_HEADER_INSERT

#endif
