#include "DataLinkLayer.h"
#include "SocketConnection.h"

#include <stdio.h>

static char queue_buffer[QUEUE_BUFFER_SIZE];
static int operation_mode;

static char frame_buffer[MESSAGE_PDU_SIZE + 9];
static const int PDU_SIZE = MESSAGE_PDU_SIZE + 9;

void initialize_dll(int operation_mode, char * host_address, char * host_port, char * receiver_address, char * receiver_port)
{
    initialize_socket(host_address, host_port, receiver_address, receiver_port);
}

void get_data_from_queue()
{

}

int send_frame()
{
    return send_data_through_socket(frame_buffer, PDU_SIZE);
}

int get_confirmation_frame()
{
    return receive_data_through_socket(frame_buffer, PDU_SIZE);
}

void pack_message_from_queue_buffer()
{

}

void unpack_message_from_frame_buffer()
{

}