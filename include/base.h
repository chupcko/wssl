#ifndef _BASE_H_
#define _BASE_H_

_INCLUDE_BEGIN_

#define WSSL_NO_DESCRIPTOR      (-1)
#define WSSL_NULL               NULL
#define WSSL_OCTET_SIZE_IN_BITS 8
#define WSSL_IP_SIZE_IN_CHAR    ((3+1)*4)

typedef uint8_t wssl_octet_t;
#define WSSL_PRINT_OCTET "02" PRIx8

typedef size_t wssl_size_t;
#define WSSL_PRINT_SIZE "zu"

typedef ssize_t wssl_ssize_t;
#define WSSL_PRINT_SSIZE "zd"

typedef struct wssl_string_t
{
  char*       data;
  wssl_size_t data_length;
} wssl_string_t;

#define WSSL_MAKE_STRING(what_data, what_data_length) \
(                                                     \
  (wssl_string_t)                                     \
  {                                                   \
    .data = (what_data),                              \
    .data_length = (what_data_length)                 \
  }                                                   \
)                                                     \

_INCLUDE_END_

static inline
void wssl_string_init
(
  _WSSL_MODIFY_ wssl_string_t* string
)
{
  string->data = WSSL_NULL;
  string->data_length = 0;
}

static inline
bool wssl_string_is_allocated
(
  _WSSL_IN_ const wssl_string_t* string
)
{
  return string->data != WSSL_NULL;
}

static inline
bool wssl_string_is_not_allocated
(
  _WSSL_IN_ const wssl_string_t* string
)
{
  return string->data == WSSL_NULL;
}

static inline
void wssl_string_print
(
  _WSSL_IN_     const wssl_string_t* string,
  _WSSL_MODIFY_       FILE*          file
)
{
  if(wssl_string_is_allocated(string))
    fprintf
    (
      file,
      "\"%s\":%" WSSL_PRINT_SIZE,
      string->data,
      string->data_length
    );
  else
    fprintf(file, "none");
}

static inline
void wssl_string_free
(
  _WSSL_MODIFY_ wssl_string_t* string
)
{
  free((void*)string->data);
  string->data = WSSL_NULL;
  string->data_length = 0;
}

#define SHA1_RESULT_SIZE_IN_BITS   160
#define SHA1_RESULT_SIZE_IN_OCTETS (SHA1_RESULT_SIZE_IN_BITS/WSSL_OCTET_SIZE_IN_BITS)
typedef wssl_octet_t wssl_sha1_result_t[SHA1_RESULT_SIZE_IN_OCTETS];

#define BASE64_ENCODE_RESULT_SIZE(x) (((x)+2)/3*4)

#define HANDSHAKE_MAGIC       "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define HANDSHAKE_RESULT_SIZE (BASE64_ENCODE_RESULT_SIZE(SHA1_RESULT_SIZE_IN_OCTETS)+1)

#define INDENT_SIZE_IN_CHAR 2
#define INDENT_FORMAT       "%*s"
#define INDENT(what_level)  (int)((what_level)*INDENT_SIZE_IN_CHAR), ""

#endif
