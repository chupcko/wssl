#ifndef _HEADER_H_
#define _HEADER_H_

_INCLUDE_BEGIN_

typedef struct wssl_header_field_t
{
  wssl_header_field_chain_t chain_link;
  wssl_string_t             key;
  wssl_string_t             value;
  char                      key_data[];
} wssl_header_field_t;

typedef struct wssl_header_t
{
  wssl_string_t             method;
  wssl_string_t             uri;
  wssl_string_t             version;
  wssl_header_field_chain_t fields;
} wssl_header_t;

_INCLUDE_END_

MAKE_CHAIN_ENTRY(wssl_header_field, wssl_header_field_chain_t, wssl_header_field_t, chain_link)

static inline
void wssl_header_init
(
  _WSSL_MODIFY_ wssl_header_t* header
)
{
  wssl_string_init(&header->method);
  wssl_string_init(&header->uri);
  wssl_string_init(&header->version);
  wssl_header_field_chain_root_init(&header->fields);
}

#define MAKE_HEADER_INSERT(what_member)                                        \
static inline                                                                  \
wssl_result_t wssl_header_insert_##what_member                                 \
(                                                                              \
  _WSSL_MODIFY_       wssl_header_t* header,                                   \
  _WSSL_IN_           char*          data,                                     \
  _WSSL_IN_     const wssl_size_t    data_size                                 \
)                                                                              \
{                                                                              \
  header->what_member.data = (char*)malloc((size_t)(data_size+1));             \
  if(header->what_member.data == NULL)                                         \
    return MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_" #what_member); \
  strncpy(header->what_member.data, data, data_size);                          \
  header->what_member.data[data_size] = '\0';                                  \
  header->what_member.data_length = data_size;                                 \
  PASS;                                                                        \
}                                                                              \

MAKE_HEADER_INSERT(method)
MAKE_HEADER_INSERT(uri)
MAKE_HEADER_INSERT(version)

static inline
wssl_result_t wssl_header_add_field
(
  _WSSL_MODIFY_       wssl_header_t*        header,
  _WSSL_IN_           char*                 key_data,
  _WSSL_IN_     const wssl_size_t           key_data_size
)
{
  wssl_header_field_t* header_field = (wssl_header_field_t*)malloc
  (
    sizeof(wssl_header_field_t)+
    (size_t)(key_data_size+1)
  );
  if(header_field == NULL)
    return MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_field");

  header_field->key.data = header_field->key_data;
  strncpy(header_field->key.data, key_data, key_data_size);
  header_field->key.data[key_data_size] = '\0';
  header_field->key.data_length = key_data_size;

  wssl_string_init(&header_field->value);

  wssl_header_field_chain_add_link_backward(&header->fields, &header_field->chain_link);

  PASS;
}

static inline
wssl_result_t wssl_header_insert_value_at_last_field
(
  _WSSL_MODIFY_ wssl_header_t* header,
  _WSSL_IN_     char*          data,
  _WSSL_IN_     wssl_size_t    data_size
)
{
  WSSL_ASSERT(wssl_header_field_chain_is_not_empty(&header->fields));
  wssl_header_field_t* header_field = wssl_header_field_chain_get_entry_from_chain_link(header->fields.prev);

  header_field->value.data = (char*)malloc((size_t)(data_size+1));
  if(header_field->value.data == NULL)
    return MAKE_RESULT(WSSL_RESULT_CODE_ERROR_MEMORY, "header_field_key");
  strncpy(header_field->value.data, data, data_size);
  header_field->value.data[data_size] = '\0';
  header_field->value.data_length = data_size;

  PASS;
}

static inline
void wssl_header_field_delete
(
  _WSSL_MODIFY_ wssl_header_field_t* header_field
)
{
  if(wssl_string_is_allocated(&header_field->value))
    wssl_string_free(&header_field->value);
  wssl_header_field_chain_delete_link(&header_field->chain_link);
  free((void*)header_field);
}

#endif
