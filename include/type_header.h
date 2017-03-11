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

#endif
