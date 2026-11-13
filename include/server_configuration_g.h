#ifndef _SERVER_CONFIGURATION_G_H_
#define _SERVER_CONFIGURATION_G_H_

#define MAKE_WSSL_SERVER_CONFIGURATION_SET(what_type, what_name) \
static inline                                                    \
void wssl_server_configuration_set_##what_name                   \
(                                                                \
  _WSSL_MODIFY_ wssl_server_configuration_t* configuration,      \
  _WSSL_IN_     what_type                    what_name           \
)                                                                \
{                                                                \
  configuration->what_name = what_name;                          \
}                                                                \

_INCLUDE_BEGIN_

MAKE_WSSL_SERVER_CONFIGURATION_SET(int,  port           )
MAKE_WSSL_SERVER_CONFIGURATION_SET(bool, ipv6           )
MAKE_WSSL_SERVER_CONFIGURATION_SET(bool, ipv6_dual_stack)
MAKE_WSSL_SERVER_CONFIGURATION_SET(bool, ssl            )

_INCLUDE_END_

#endif
