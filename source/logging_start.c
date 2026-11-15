#include "main.h"

_LIBRARY_FUNCTION_
wssl_result_t wssl_logging_start
(
  _WSSL_MODIFY_ wssl_t* wssl
)
{
  if
  (
    wssl->syslog_server == WSSL_NULL ||
    wssl->logging.descriptor != WSSL_NO_DESCRIPTOR
  )
  {
    PASS;
  }

  if(wssl->syslog_ident != WSSL_NULL)
  {
    wssl->logging.ident = wssl->syslog_ident;
  }
  else
  {
    wssl->logging.ident = wssl->server_name;
  }

  if(gethostname(wssl->logging.hostname, sizeof(wssl->logging.hostname)-1) != 0)
  {
    wssl->logging.hostname[0] = '-';
    wssl->logging.hostname[1] = '\0';
  }
  else
  {
    wssl->logging.hostname[sizeof(wssl->logging.hostname)-1] = '\0';
  }

  if(wssl->syslog_server[0] == '/')
  {
    int syslog_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(syslog_descriptor < 0)
    {
      int system_errno = errno;
      FAIL_ERRNO("socket", system_errno);
    }

    if(strlen(wssl->syslog_server) >= sizeof(((struct sockaddr_un*)0)->sun_path))
    {
      close(syslog_descriptor);
      FAIL_ERROR("syslog_server", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
    }

    struct sockaddr_un syslog_address;
    syslog_address.sun_family = AF_UNIX;
    wssl_str_copy(syslog_address.sun_path, sizeof(syslog_address.sun_path), wssl->syslog_server);

    if(connect(syslog_descriptor, (struct sockaddr*)&syslog_address, sizeof(syslog_address)) < 0)
    {
      int system_errno = errno;
      close(syslog_descriptor);
      FAIL_ERRNO("connect", system_errno);
    }

    wssl->logging.descriptor = syslog_descriptor;
    PASS;
  }

  char* last_colon = strrchr(wssl->syslog_server, ':');
  if(last_colon == NULL || last_colon == wssl->syslog_server || *(last_colon+1) == '\0')
  {
    FAIL_ERROR("syslog_server", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
  }

  char* port_end = NULL;
  long port_value = strtol(last_colon+1, &port_end, 10);
  if(port_end == last_colon+1 || *port_end != '\0' || port_value < 1 || port_value > WSSL_MAX_PORT_NUMBER)
  {
    FAIL_ERROR("syslog_server", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
  }
  int syslog_port = (int)port_value;

  size_t host_size = (size_t)(last_colon-wssl->syslog_server);
  if(host_size >= WSSL_IP_SIZE_IN_CHAR)
  {
    FAIL_ERROR("syslog_server", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
  }
  char ip_copy[WSSL_IP_SIZE_IN_CHAR];
  wssl_str_copy(ip_copy, WSSL_IP_SIZE_IN_CHAR, wssl->syslog_server);
  ip_copy[host_size] = '\0';

  struct sockaddr_in6 syslog_address;
  struct sockaddr_in syslog_address4;

  bool is_ipv6 = false;
  int return_value = inet_pton(AF_INET6, ip_copy, (void*)&syslog_address.sin6_addr);
  if(return_value < 0)
  {
    int system_errno = errno;
    FAIL_ERRNO("inet_pton", system_errno);
  }
  if(return_value == 0)
  {
    return_value = inet_pton(AF_INET, ip_copy, (void*)&syslog_address4.sin_addr);
    if(return_value < 0)
    {
      int system_errno = errno;
      FAIL_ERRNO("inet_pton", system_errno);
    }
    if(return_value == 0)
    {
      FAIL_ERROR("syslog_server", WSSL_RESULT_CODE_ERROR_BAD_SYNTAX);
    }
  }
  else
  {
    is_ipv6 = true;
  }

  int syslog_descriptor;
  if(is_ipv6)
  {
    syslog_descriptor = socket(AF_INET6, SOCK_DGRAM, 0);
    if(syslog_descriptor < 0)
    {
      int system_errno = errno;
      FAIL_ERRNO("socket", system_errno);
    }
   
    syslog_address.sin6_family = AF_INET6;
    syslog_address.sin6_port = htons(syslog_port);
    syslog_address.sin6_flowinfo = 0;
    syslog_address.sin6_scope_id = 0;
    if(connect(syslog_descriptor, (struct sockaddr*)&syslog_address, sizeof(syslog_address)) < 0)
    {
      int system_errno = errno;
      close(syslog_descriptor);
      FAIL_ERRNO("connect", system_errno);
    }
  }
  else
  {
    syslog_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(syslog_descriptor < 0)
    {
      int system_errno = errno;
      FAIL_ERRNO("socket", system_errno);
    }

    syslog_address4.sin_family = AF_INET;
    syslog_address4.sin_port = htons(syslog_port);
    if(connect(syslog_descriptor, (struct sockaddr*)&syslog_address4, sizeof(syslog_address4)) < 0)
    {
      int system_errno = errno;
      close(syslog_descriptor);
      FAIL_ERRNO("connect", system_errno);
    }
  }

  wssl->logging.descriptor = syslog_descriptor;
 
 PASS;
}
