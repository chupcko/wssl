#include "main.h"

_LIBRARY_FUNCTION_
void wssl_logging
(
  _WSSL_MODIFY_ wssl_t*              wssl,
  _WSSL_IN_     wssl_logging_level_e level,
  _WSSL_IN_     char*                where,
  _WSSL_IN_     char*                message
)
{
  if(wssl->logging_to_stderr == true)
  {
    fprintf(stderr, "[%s] %s: %s\n", wssl_logging_level_get_name(level), where, message);
  }

  if(level > wssl->logging_level)
  {
    return;
  }

  if(wssl->logging.descriptor == WSSL_NO_DESCRIPTOR)
  {
    return;
  }

  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  struct tm broken;
  gmtime_r(&now.tv_sec, &broken);
  char timestamp[WSSL_TIMESTAMP_SIZE_IN_CHAR];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", &broken);

  char packet[SYSLOG_PACKET_SIZE_IN_OCTETS];
  int packet_size = snprintf(
    packet,
    sizeof(packet),
    "<%d>1 %s.%03ldZ %s %s %d - - [%s] %s: %s",
    SYSLOG_DAEMON_FACILITY+(int)level,
    timestamp,
    now.tv_nsec/1000000,
    wssl->logging.hostname,
    wssl->logging.ident,
    (int)getpid(),
    wssl_logging_level_get_name(level),
    where,
    message
  );
  if(packet_size <= 0)
  {
    return;
  }
  if(packet_size > (int)sizeof(packet)-1)
  {
    packet_size = (int)sizeof(packet)-1;
  }

  if(send(wssl->logging.descriptor, packet, (size_t)packet_size, 0) < 0)
  {
    if(wssl->logging_to_stderr == true)
    {
      fprintf(stderr, "[%s] logging: syslog send failed\n", wssl_logging_level_get_name(WSSL_LOGGING_LEVEL_ALERT));
    }
  }
}
