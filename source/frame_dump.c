#include "main.h"

_LIBRARY_FUNCTION_
void wssl_frame_dump
(
  _WSSL_IN_ const wssl_frame_t* frame,
  _WSSL_IN_       FILE*         file,
  _WSSL_IN_ const wssl_size_t   indent_level
)
{
  wssl_size_t data_index;

  fprintf
  (
    file,
    (
      INDENT_FORMAT "frame=\n"
      INDENT_FORMAT "fin: %s\n"
      INDENT_FORMAT "opcode: %" WSSL_PRINT_OCTET "\n"
      INDENT_FORMAT "masked: %s\n"
      INDENT_FORMAT "length: %" WSSL_PRINT_OCTET "\n"
      INDENT_FORMAT "masking_key:"
    ),
    INDENT(indent_level),
    INDENT(indent_level+1), wssl_get_bool_string(frame->fin),
    INDENT(indent_level+1), frame->opcode,
    INDENT(indent_level+1), wssl_get_bool_string(frame->masked),
    INDENT(indent_level+1), frame->length,
    INDENT(indent_level+1)
  );
  if(frame->masked)
    for(data_index = 0; data_index < WSSL_FRAME_MASKING_KEY_SIZE; data_index++)
      fprintf(file, " %" WSSL_PRINT_OCTET, frame->masking_key[data_index]);
  else
    fprintf(file, " none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "payload: "
    ),
    INDENT(indent_level+1)
  );
  if(frame->payload != WSSL_NULL)
  {
    fprintf(file, "\"");
    for(data_index = 0; data_index < frame->payload_size; data_index++)
    {
      if(data_index != 0)
        fprintf(file, " ");
      fprintf(file, "%" WSSL_PRINT_OCTET, frame->payload[data_index]);
    }
    fprintf(file, "\"");
  }
  else
    fprintf(file, "none");
  fprintf
  (
    file,
    (
      "\n"
      INDENT_FORMAT "payload_size: %" WSSL_PRINT_FRAME_SIZE "\n"
    ),
    INDENT(indent_level+1), frame->payload_size
  );
}
