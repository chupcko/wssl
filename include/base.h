#ifndef _BASE_H_
#define _BASE_H_

_INCLUDE_BEGIN_

#define WSSL_NO_DESCRIPTOR      (-1)
#define WSSL_NULL               NULL
#define WSSL_OCTET_SIZE_IN_BITS 8
#define WSSL_IP_SIZE_IN_CHAR    ((3+1)*4)

typedef uint8_t wssl_octet_t;
typedef size_t wssl_size_t;
typedef ssize_t wssl_ssize_t;

#define WSSL_SHA1_RESULT_SIZE_IN_BITS   160
#define WSSL_SHA1_RESULT_SIZE_IN_OCTETS (WSSL_SHA1_RESULT_SIZE_IN_BITS/WSSL_OCTET_SIZE_IN_BITS)

typedef wssl_octet_t wssl_sha1_result_t[WSSL_SHA1_RESULT_SIZE_IN_OCTETS];

_INCLUDE_END_

#define HANDSHAKE_MAGIC       "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define INDENT_SIZE_IN_CHAR 2
#define INDENT_FORMAT       "%*s"
#define INDENT(what_level)  ((what_level)*INDENT_SIZE_IN_CHAR), ""

#endif
