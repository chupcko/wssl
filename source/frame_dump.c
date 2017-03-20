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
      INDENT_FORMAT "opcode: %01" PRIx8 "\n"
      INDENT_FORMAT "masked: %s\n"
      INDENT_FORMAT "length: %02" PRIx8 "\n"
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
      fprintf(file, " %02" PRIx8, frame->masking_key[data_index]);
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
    wssl_size_t data_index;
    for(data_index = 0; data_index < frame->payload_size; data_index++)
    {
      if(data_index != 0)
        fprintf(file, " ");
      fprintf(file, "%02" PRIx8, frame->payload[data_index]);
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
      INDENT_FORMAT "payload_size: %08" PRId64 "\n"
      INDENT_FORMAT "count: %d\n"
    ),
    INDENT(indent_level+1), frame->payload_size,
    INDENT(indent_level+1), frame->count
  );
}