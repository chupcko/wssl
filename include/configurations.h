#ifndef _CONFIGURATIONS_H_
#define _CONFIGURATIONS_H_

_INCLUDE_BEGIN_

#define WSSL_DEFAULT_SERVER_NAME                         ("WSSL-" WSSL_VERSION)
#define WSSL_DEFAULT_BUFFER_SIZE_IN_OCTETS               4096
#define WSSL_DEFAULT_SLEEP_IN_MSECONDS                   1000
#define WSSL_DEFAULT_TIMEOUT_IN_MSECONDS                 0
#define WSSL_DEFAULT_PING_INTERVAL_IN_MSECONDS           0
#define WSSL_DEFAULT_PONG_TIMEOUT_IN_MSECONDS            0
#define WSSL_DEFAULT_MAX_NUMBER_OF_RECEIVED_MULTI_FRAMES 8
#define WSSL_DEFAULT_MAX_NUMBER_OF_HEADER_FIELDS         64
#define WSSL_DEFAULT_MAX_NUMBER_OF_OUTPUT_CHUNKS         16
#define WSSL_DEFAULT_LOGGING_TO_STDERR                   true
#define WSSL_DEFAULT_SYSLOG_SERVER                       NULL
#define WSSL_DEFAULT_SYSLOG_IDENT                        NULL
#define WSSL_DEFAULT_LOGGING_LEVEL                       WSSL_LOGGING_LEVEL_ERROR

_INCLUDE_END_

#define SERVER_BACKLOG_SIZE 16
#define EPOLL_EVENTS_SIZE   16

#define WSSL_SSL_MIN_PROTOCOL_VERSION TLS1_2_VERSION
#define WSSL_SSL_CIPHER_LIST          "ECDHE-ECDSA-AES256-GCM-SHA384:" \
                                      "ECDHE-RSA-AES256-GCM-SHA384:"   \
                                      "ECDHE-ECDSA-AES128-GCM-SHA256:" \
                                      "ECDHE-RSA-AES128-GCM-SHA256"    \

#define WSSL_SSL_CIPHER_SUITES        "TLS_AES_256_GCM_SHA384:" \
                                      "TLS_AES_128_GCM_SHA256"  \

#endif
