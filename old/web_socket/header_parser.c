#include "main.h"

int header_parser(uint8_t* buffer, int buffer_length, header_data_t header_data[])
{
  int buffer_begin = 0;
  int line_length;

  while(buffer_begin < buffer_length)
  {
    line_length = 0;
    while
    (
      buffer_begin+line_length < buffer_length &&
      buffer[buffer_begin+line_length] != '\n'
    )
    {
      if(buffer[buffer_begin+line_length] == '\r')
        buffer[buffer_begin+line_length] = '\0';
      line_length++;
    }
    if(buffer[buffer_begin+line_length] == '\n')
    {
      buffer[buffer_begin+line_length] = '\0';
      line_length++;
      if(buffer[buffer_begin] == '\0')
      {
        debug("End of header");
        header_data[HEADER_INDEX_END].value = &buffer[buffer_begin];
        header_data[HEADER_INDEX_END].value_length = 0;
      }
      else
      {
        int i;

        for(i = 0; i < HEADER_INDEX_END; i++)
          if(strncmp(Header_keys[i].name, &buffer[buffer_begin], Header_keys[i].name_length) == 0)
          {
            header_data[i].value = &buffer[buffer_begin+Header_keys[i].name_length];
            header_data[i].value_length = strlen(header_data[i].value);
            debug("Header key=\"%s\" value=\"%s\"", Header_keys[i].name, header_data[i].value);
            break;
          }
      }
      buffer_begin += line_length;
    }
    else
      break;
  }
  return buffer_begin;
}
