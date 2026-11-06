#ifndef _SERVER_CONFIGURATION_H_
#define _SERVER_CONFIGURATION_H_

_INCLUDE_BEGIN_

typedef struct wssl_server_configuration_t
{
  char ip[WSSL_IP_SIZE_IN_CHAR];
  int  port;
  bool ipv6;
  bool ipv6_dual_stack;
  bool ssl;
  char certificate_file[WSSL_FILE_PATH_SIZE_IN_CHAR];
  char private_key_file[WSSL_FILE_PATH_SIZE_IN_CHAR];
} wssl_server_configuration_t;


#define WSSL_SERVER_CONFIGURATION_DEFAULT \
  (wssl_server_configuration_t)           \
  {                                       \
    .ip = "",                             \
    .port = 0,                            \
    .ipv6 = false,                        \
    .ipv6_dual_stack = false,             \
    .ssl = false,                         \
    .certificate_file = "",               \
    .private_key_file = ""                \
  }                                       \

#define WSSL_SERVER_CONFIGURATION_IPV4 \
  (wssl_server_configuration_t)        \
  {                                    \
    .ip = "0.0.0.0",                   \
    .port = 0,                         \
    .ipv6 = false,                     \
    .ipv6_dual_stack = false,          \
    .ssl = false,                      \
    .certificate_file = "",            \
    .private_key_file = ""             \
  }                                    \

#define WSSL_SERVER_CONFIGURATION_IPV6 \
  (wssl_server_configuration_t)        \
  {                                    \
    .ip = "::",                        \
    .port = 0,                         \
    .ipv6 = true,                      \
    .ipv6_dual_stack = false,          \
    .ssl = false,                      \
    .certificate_file = "",            \
    .private_key_file = ""             \
  }                                    \

#define WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK \
  (wssl_server_configuration_t)                   \
  {                                               \
    .ip = "::",                                   \
    .port = 0,                                    \
    .ipv6 = true,                                 \
    .ipv6_dual_stack = true,                      \
    .ssl = false,                                 \
    .certificate_file = "",                       \
    .private_key_file = ""                        \
  }                                               \

#define WSSL_SERVER_CONFIGURATION_IPV4_SSL \
  (wssl_server_configuration_t)            \
  {                                        \
    .ip = "0.0.0.0",                       \
    .port = 0,                             \
    .ipv6 = false,                         \
    .ipv6_dual_stack = false,              \
    .ssl = true,                           \
    .certificate_file = "",                \
    .private_key_file = ""                 \
  }                                        \

#define WSSL_SERVER_CONFIGURATION_IPV6_SSL \
  (wssl_server_configuration_t)            \
  {                                        \
    .ip = "::",                            \
    .port = 0,                             \
    .ipv6 = true,                          \
    .ipv6_dual_stack = false,              \
    .ssl = true,                           \
    .certificate_file = "",                \
    .private_key_file = ""                 \
  }                                        \

#define WSSL_SERVER_CONFIGURATION_IPV6_DUAL_STACK_SSL \
  (wssl_server_configuration_t)                       \
  {                                                   \
    .ip = "::",                                       \
    .port = 0,                                        \
    .ipv6 = true,                                     \
    .ipv6_dual_stack = true,                          \
    .ssl = true,                                      \
    .certificate_file = "",                           \
    .private_key_file = ""                            \
  }                                                   \

_INCLUDE_END_

#endif
