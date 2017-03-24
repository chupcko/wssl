#ifndef _HEADER_H_
#define _HEADER_H_

_INCLUDE_BEGIN_

typedef struct wssl_header_field_t
{
  wssl_header_field_chain_t chain_link;
  char*                     key;
  char*                     value;
} wssl_header_field_t;

typedef struct wssl_header_t
{
  char*                     method;
  char*                     uri;
  char*                     version;
  wssl_header_field_chain_t fields;
} wssl_header_t;

_INCLUDE_END_

static inline
void wssl_header_init
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  header->method = WSSL_NULL;
  header->uri = WSSL_NULL;
  header->version = WSSL_NULL;
  wssl_header_field_chain_init(&header->fields);
}

#define MAKE_HEADER_INSERT(what_member)                                                \
static inline                                                                          \
wssl_result_t wssl_header_insert_##what_member                                         \
(                                                                                      \
  _WSSL_MODIFY_ wssl_header_t* header,                                                 \
  _WSSL_IN_     char*          data,                                                   \
  _WSSL_IN_     wssl_size_t    data_size                                               \
)                                                                                      \
{                                                                                      \
  header->what_member = (char*)malloc((size_t)(data_size+1));                          \
  if(header->what_member == NULL)                                                      \
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_" #what_member, 0); \
  strncpy(header->what_member, data, data_size);                                       \
  header->what_member[data_size] = '\0';                                               \
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);                          \
}                                                                                      \

MAKE_HEADER_INSERT(method)
MAKE_HEADER_INSERT(uri)
MAKE_HEADER_INSERT(version)
#undef MAKE_HEADER_INSERT

static inline
wssl_result_t wssl_header_add_field
(
  _WSSL_MODIFY_ wssl_header_t* header,
  _WSSL_IN_     char*          data,
  _WSSL_IN_     wssl_size_t    data_size
)
{
  wssl_header_field_t* header_field = (wssl_header_field_t*)malloc(sizeof(wssl_header_field_t));
  if(header_field == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_field", 0);

  header_field->key = (char*)malloc((size_t)(data_size+1));
  if(header_field->key == NULL)
  {
    free((void*)header_field);
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_field_key", 0);
  }
  strncpy(header_field->key, data, data_size);
  header_field->key[data_size] = '\0';

  header_field->value = WSSL_NULL;

  wssl_header_field_chain_add_link_backward(&header->fields, &header_field->chain_link);

  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

static inline
wssl_result_t wssl_header_field_insert_value
(
  _WSSL_MODIFY_ wssl_header_field_t* header_field,
  _WSSL_IN_     char*                data,
  _WSSL_IN_     wssl_size_t          data_size
)
{
  header_field->value = (char*)malloc((size_t)(data_size+1));
  if(header_field->value == NULL)
    return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_field_key", 0);
  strncpy(header_field->value, data, data_size);
  header_field->value[data_size] = '\0';
  return WSSL_MAKE_RESULT(WSSL_RESULT_CODE_OK, WSSL_NULL, 0);
}

#endif
