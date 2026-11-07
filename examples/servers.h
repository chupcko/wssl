#ifndef _EXAMPLES_SERVERS_H_
#define _EXAMPLES_SERVERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wssl.h"

#define SERVERS_MAX_COUNT 16
#define SERVERS_LINE_SIZE 1024

int servers_parse_port
(
  const char* text,
  int*        port
)
{
  char* end = NULL;
  long value = strtol(text, &end, 10);
  if(end == text || *end != '\0' || value < 1 || value > 65535)
  {
    return -1;
  }
  *port = (int)value;
  return 0;
}

int servers_parse_stack
(
  const char*                  text,
  wssl_server_configuration_t* configuration
)
{
  if(strcmp(text, "ipv4") == 0)
  {
    configuration->ipv6 = false;
    configuration->ipv6_dual_stack = false;
  }
  else if(strcmp(text, "ipv6") == 0)
  {
    configuration->ipv6 = true;
    configuration->ipv6_dual_stack = false;
  }
  else if(strcmp(text, "ipv6-dual-stack") == 0)
  {
    configuration->ipv6 = true;
    configuration->ipv6_dual_stack = true;
  }
  else
  {
    return -1;
  }
  return 0;
}

int servers_copy_text
(
  char*       destination,
  const char* source,
  size_t      destination_size
)
{
  int result = snprintf(destination, destination_size, "%s", source);
  if(result < 0 || result >= (int)destination_size)
  {
    return -1;
  }
  return 0;
}

int servers_read_configurations
(
  const char*                  file_name,
  wssl_server_configuration_t* servers
)
{
  FILE* file = fopen(file_name, "r");
  if(file == NULL)
  {
    fprintf(stderr, "cannot open '%s'\n", file_name);
    return -1;
  }

  char line[SERVERS_LINE_SIZE];
  int line_number = 0;
  int server_count = 0;
  while(fgets(line, sizeof(line), file) != NULL)
  {
    line_number++;

    char* rest = NULL;
    char* stack = strtok_r(line, " \t\r\n", &rest);
    if(stack == NULL || stack[0] == '#')
    {
      continue;
    }
    char* ip = strtok_r(NULL, " \t\r\n", &rest);
    char* port_text = strtok_r(NULL, " \t\r\n", &rest);
    char* ssl = strtok_r(NULL, " \t\r\n", &rest);
    char* certificate_file = NULL;
    char* private_key_file = NULL;
    if(ssl != NULL)
    {
      certificate_file = strtok_r(NULL, " \t\r\n", &rest);
      private_key_file = strtok_r(NULL, " \t\r\n", &rest);
    }
    char* extra = strtok_r(NULL, " \t\r\n", &rest);
    if(ip == NULL || port_text == NULL || extra != NULL)
    {
      fprintf(stderr, "%s:%d: bad line, expected '<stack> <ip> <port> [ssl <certificate> <key>]'\n", file_name, line_number);
      fclose(file);
      return -1;
    }

    if(server_count >= SERVERS_MAX_COUNT)
    {
      fprintf(stderr, "%s:%d: too many servers, max is %d\n", file_name, line_number, SERVERS_MAX_COUNT);
      fclose(file);
      return -1;
    }

    wssl_server_configuration_t* configuration = &servers[server_count];
    *configuration = WSSL_SERVER_CONFIGURATION_DEFAULT;

    if(servers_parse_stack(stack, configuration) != 0)
    {
      fprintf(stderr, "%s:%d: bad stack '%s', expected 'ipv4', 'ipv6' or 'ipv6-dual-stack'\n", file_name, line_number, stack);
      fclose(file);
      return -1;
    }
    if(servers_copy_text(configuration->ip, ip, sizeof(configuration->ip)) != 0)
    {
      fprintf(stderr, "%s:%d: bad ip '%s'\n", file_name, line_number, ip);
      fclose(file);
      return -1;
    }
    if(servers_parse_port(port_text, &configuration->port) != 0)
    {
      fprintf(stderr, "%s:%d: bad port '%s'\n", file_name, line_number, port_text);
      fclose(file);
      return -1;
    }
    if(ssl != NULL)
    {
      if(strcmp(ssl, "ssl") != 0 || certificate_file == NULL || private_key_file == NULL)
      {
        fprintf(stderr, "%s:%d: bad ssl part, expected 'ssl <certificate> <key>'\n", file_name, line_number);
        fclose(file);
        return -1;
      }
      if
      (
        servers_copy_text(configuration->certificate_file, certificate_file, sizeof(configuration->certificate_file)) != 0 ||
        servers_copy_text(configuration->private_key_file, private_key_file, sizeof(configuration->private_key_file)) != 0
      )
      {
        fprintf(stderr, "%s:%d: bad certificate file names\n", file_name, line_number);
        fclose(file);
        return -1;
      }
      configuration->ssl = true;
    }

    server_count++;
  }

  fclose(file);

  if(server_count == 0)
  {
    fprintf(stderr, "%s: no servers configured\n", file_name);
    return -1;
  }

  return server_count;
}

#endif
