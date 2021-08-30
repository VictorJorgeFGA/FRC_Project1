#include "DataLinkLayer.h"
#include "SocketConnection.h"
#include "DataLinkLayerInterface.h"
#include "Params.h"

#include <stdio.h>
#include <string.h>

#define WAITING_APP 0
#define SENDING 1
#define RECEIVING 2
#define WAITING_PEER 3

static int operation_mode;

static char queue_buffer[CQ_DATA_MAX_LEN];

static char incoming_frame_buffer[PDU_SIZE];
static long long incoming_frame_id = 0;

static char outcoming_frame_buffer[PDU_SIZE];
static long long outcoming_frame_id = 0;

static int layer_state;

void initialize_dll(int operation_mode, char * host_address, char * host_port, char * receiver_address, char * receiver_port)
{
    initialize_socket(host_address, host_port, receiver_address, receiver_port);
    if (operation_mode == SENDER)
        layer_state = WAITING_APP;
    else
        layer_state = WAITING_PEER;
}

void run_dll()
{
    while (operation_mode == SENDER) {
        get_data_from_queue();
        pack_message_from_queue_buffer();
        delivery_frame();
    }

    while (operation_mode == RECEIVER) {
        get_data_from_sender();
        unpack_message_from_frame_buffer();
        send_data_to_queue();
    }
}

void get_data_from_queue()
{
    int nothing;
    get_data_from_app(queue_buffer, &nothing);
}

void send_data_to_queue()
{
    send_data_to_app(queue_buffer, CQ_DATA_MAX_LEN);
}

int check_incoming_frame()
{
    long long received_id = *((long long *)incoming_frame_buffer);
    if (received_id != incoming_frame_id) {
        printf("Package ID differs from expected\nExpected: %lld, actual: %lld\n", incoming_frame_id, received_id);
        return 1;
    }

    // TODO implementar checksum
    // ------------

    incoming_frame_id++;
    return 0;
}

int send_error_confirmation_frame()
{
    memset(outcoming_frame_buffer, 0xFF, PDU_SIZE);
    return send_frame();
}

int send_ok_confirmation_frame()
{
    memset(outcoming_frame_buffer, 0x0, PDU_SIZE);
    return send_frame();
}

void delivery_frame()
{
    while (send_frame());
    while (get_confirmation_frame());
}

int send_frame()
{
    return send_data_through_socket(outcoming_frame_buffer, PDU_SIZE);
}

int get_confirmation_frame()
{
    return receive_data_through_socket(incoming_frame_buffer, PDU_SIZE);
}

int receive_frame()
{
    memset(incoming_frame_buffer, 0x0, PDU_SIZE);
    return receive_data_through_socket(incoming_frame_buffer, PDU_SIZE);
}

void get_data_from_sender()
{
    while (receive_frame());
    // Caso o frame recebido esteja corrompido, tentar enviar mensage de erro para o par
    if (check_incoming_frame())
        while (send_error_confirmation_frame());

    // Caso o frame recebido esteja ok, tentar enviar mensagem de ok para o par
    else
        while (send_ok_confirmation_frame());
}

void pack_message_from_queue_buffer()
{
    *((long long *) outcoming_frame_buffer) = outcoming_frame_id++;
    for (int i = HEADER_PDU_SIZE; i < PDU_SIZE; i++)
        outcoming_frame_buffer[i] = queue_buffer[i - HEADER_PDU_SIZE];
}

void unpack_message_from_frame_buffer()
{
    for (int i = HEADER_PDU_SIZE; i < PDU_SIZE; i++)
        queue_buffer[i - HEADER_PDU_SIZE] = incoming_frame_buffer[i];
}