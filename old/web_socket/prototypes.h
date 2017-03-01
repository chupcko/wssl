#ifndef _PROTOTYPES_H_
#define _PROTOTYPES_H_

int base64_encode(uint8_t*, int, char*, int);
void callback_connect_function(client_info_t*, void*);
void callback_disconnect_function(client_info_t*, void*);
bool callback_periodic_function(client_info_t*, void*);
bool callback_poll_function(client_info_t*, void*);
bool callback_recv_function(client_info_t*, void*, uint8_t*, int);
void client_send(int, uint8_t*, int);
void connection_loop(client_info_t*);
void debug_real(char*, ...);
void error(char*, ...);
bool frame_execute(client_info_t*, frame_t*, uint8_t*, int);
void frame_header_fill_random_masking_key(frame_t*);
void frame_mask_unmask(frame_t* frame);
void frame_send(client_info_t*, uint8_t, char*, int);
void frame_send_text(client_info_t*, char*);
int get_frame_header(uint8_t*, int, frame_t*);
int handshake_calculate(char*, char*, int);
int header_parser(uint8_t*, int, header_data_t[]);
bool header_response(int, header_data_t[]);
int put_frame_header(frame_t*, uint8_t*, int);
int server_init(int);
void server_loop(int);
void set_callback_connect(callback_connect_t*);
void set_callback_disconnect(callback_disconnect_t*);
void set_callback_extra_data(void*);
void set_callback_periodic(callback_periodic_t*);
void set_callback_recv(callback_recv_t*);
void signal_handler_child_end(int);

#endif
