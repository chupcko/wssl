#ifndef _VARIABLES_H_
#define _VARIABLES_H_

_EXTERN_ header_key_t Header_keys[] _INIT_
(
  {
    { HEADER_NAME_GET,        sizeof HEADER_NAME_GET-1        },
    { HEADER_NAME_HOST,       sizeof HEADER_NAME_HOST-1       },
    { HEADER_NAME_UPGRADE,    sizeof HEADER_NAME_UPGRADE-1    },
    { HEADER_NAME_SEC_WS_KEY, sizeof HEADER_NAME_SEC_WS_KEY-1 },
    { HEADER_NAME_END,        sizeof HEADER_NAME_END-1        },
    { NULL                    -1                              }
  }
);

_EXTERN_ void* Callback_extra_data                  _INIT_(NULL);
_EXTERN_ callback_connect_t* Callback_connect       _INIT_(NULL);
_EXTERN_ callback_disconnect_t* Callback_disconnect _INIT_(NULL);
_EXTERN_ callback_periodic_t* Callback_periodic     _INIT_(NULL);
_EXTERN_ callback_poll_t* Callback_poll             _INIT_(NULL);
_EXTERN_ int Callback_poll_fd                       _INIT_(-1);
_EXTERN_ callback_recv_t* Callback_recv             _INIT_(NULL);

_EXTERN_ uint8_t Out_buffer[BUFFER_SIZE];

_EXTERN_ extra_data_t Extra_data;

#endif
