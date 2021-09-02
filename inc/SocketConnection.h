#ifndef _SOCKET_CONNECTION_H_
#define _SOCKET_CONNECTION_H_

void initialize_socket(char * host_port, char * receiver_address, char * receiver_port);
void shut_down_socket();

int send_data_through_socket(char * data_buffer, int data_buffer_size);
int receive_data_through_socket(char * data_buffer, int data_buffer_size);

#endif