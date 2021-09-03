#include "DataLinkLayer.h"
#include "SocketConnection.h"
#include "DataLinkLayerInterface.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int operation_mode;

static const int QUEUE_BUFFER_SIZE = CQ_DATA_MAX_LEN;
static int queue_buffer_pos = 0;
static char queue_buffer[CQ_DATA_MAX_LEN];

static int pdu_size;

static char * incoming_frame_buffer;
static long long incoming_frame_id = 0;

static char * outcoming_frame_buffer;
static long long outcoming_frame_id = 0;

static int layer_state;

static const char * dll_error_msg_format = "(DLL) \033[0;31mERROR:\033[0m ";
static const char * dll_warning_msg_format = "(DLL) \033[0;33mWARNING:\033[0m ";
static const char * dll_success_msg_format = "(DLL) \033[0;32mSUCCESS:\033[0m ";
static const char * dll_info_msg_format = "(DLL) \033[0;34mINFO:\033[0m ";

static int verbose = 0;

void show_buffer(char * buffer, int buffer_len)
{
    printf("%d[", buffer_len);
    for (int i = 0; i < buffer_len; i++) {
        switch (buffer[i])
        {
        case 0xFF:
            printf("1");
            break;
        case 0x0:
            printf("0");

        default:
            printf("%c", buffer[i]);
            break;
        }
    }
    printf("]\n");
}

void initialize_dll(char * host_port, char * receiver_address, char * receiver_port, int t_pdu_size)
{
    initialize_socket(host_port, receiver_address, receiver_port);
    initialize_dll_interface();

    pdu_size = CQ_DATA_MAX_LEN + PDU_HEADER_SIZE;
    for (int i = 2; i < CQ_DATA_MAX_LEN; i++) {
        if (CQ_DATA_MAX_LEN % i != 0)
            continue;

        if (abs((i + PDU_HEADER_SIZE) - t_pdu_size) < abs((pdu_size + PDU_HEADER_SIZE) - t_pdu_size))
            pdu_size = i + PDU_HEADER_SIZE;
    }

    if (CQ_DATA_MAX_LEN % (pdu_size - PDU_HEADER_SIZE) != 0) {
        printf("%sCritical error: queue message size (%d) can't be divided by PDU payload size (%d).\n", dll_error_msg_format, CQ_DATA_MAX_LEN, pdu_size - PDU_HEADER_SIZE);
        printf("%sInsert a different PDU size.\n", dll_warning_msg_format);
        exit(1);
    }

    if (pdu_size != t_pdu_size)
        printf("%sPDU size was adjusted to %d to avoid inconsistency problems.\n", dll_warning_msg_format, pdu_size);

    if ((incoming_frame_buffer = malloc((size_t) pdu_size)) == NULL) {
        printf("%sUnable to allocate memory to incoming frame buffer.\n", dll_error_msg_format);
        exit(1);
    }

    if ((outcoming_frame_buffer = malloc((size_t) pdu_size)) == NULL) {
        printf("%sUnable to allocate memory to outcoming frame buffer.\n", dll_error_msg_format);
        exit(1);
    }

    operation_mode = RECEIVER;
    printf("%sData Link Layer initialized as %s successfully!\n", dll_success_msg_format, operation_mode == SENDER ? "SENDER" : "RECEIVER");
}

void shut_down_dll()
{
    free(incoming_frame_buffer);
    free(outcoming_frame_buffer);

    shut_down_dll_interface();
    shut_down_socket();
}

void run_dll()
{
    while (operation_mode == SENDER) {
        get_data_from_queue();
        send_data();
    }

    while (operation_mode == RECEIVER) {
        get_data();
        send_data_to_queue();
    }
}

void set_verbose_dll(int value)
{
    verbose = value;
}

void set_operation_mode(int value)
{
    if (value != RECEIVER && value != SENDER) {
        printf("%sAttempt to set operation mode with invalid value: %d.\n", dll_error_msg_format, value);
        return;
    }

    operation_mode = value;

    if (verbose)
        printf("%sNow operating as %s.\n", dll_info_msg_format, operation_mode == RECEIVER ? "RECEIVER" : "SENDER");
}

void get_data_from_queue()
{
    int nothing;
    get_data_from_app(queue_buffer, &nothing);
}

void send_data_to_queue()
{
    send_data_to_app(queue_buffer, QUEUE_BUFFER_SIZE);
}

int check_incoming_frame()
{
    long long received_id = *((long long *)incoming_frame_buffer);
    if (received_id != incoming_frame_id) {
        printf("%sPackage ID differs from expected. Expected: %lld, actual: %lld\n", dll_error_msg_format, incoming_frame_id, received_id);
        return 1;
    }

    // TODO implementar CRC ou paridade
    // ------------

    incoming_frame_id++;
    return 0;
}

int send_error_confirmation_frame()
{
    memset(outcoming_frame_buffer, 0xFF, pdu_size);
    return send_frame();
}

int send_ok_confirmation_frame()
{
    memset(outcoming_frame_buffer, 0x0, pdu_size);
    return send_frame();
}

void send_data()
{
    if (verbose)
        printf("%sSending data from queue.\n", dll_info_msg_format);

    while (queue_buffer_pos < QUEUE_BUFFER_SIZE - 1) {
        pack_message_from_queue_buffer();
        send_frame_to_receiver();
    }
    queue_buffer_pos = 0;

    if (verbose)
        printf("%sData from queue sent successfully.\n\n", dll_info_msg_format);
}

void get_data()
{
    if (verbose)
        printf("%sReceiving message data.\n", dll_info_msg_format);

    while (queue_buffer_pos < QUEUE_BUFFER_SIZE - 1) {
        get_frame_from_sender();
        unpack_message_from_frame_buffer();
    }
    queue_buffer_pos = 0;

    if (verbose)
        printf("%sMessage data received successfully.\n\n", dll_info_msg_format);
}

void send_frame_to_receiver()
{
    do {
        if (verbose)
            printf("%sSending frame %lld.\n", dll_info_msg_format, outcoming_frame_id);

        while (send_frame())
            printf("%sFailed to send frame %lld. Trying again...\n", dll_error_msg_format, outcoming_frame_id);

        while (get_confirmation_frame())
            printf("%sFailed to receive confirmation frame. Trying again...\n", dll_error_msg_format);

    } while (check_confirmation_frame());
}

int send_frame()
{
    return send_data_through_socket(outcoming_frame_buffer, pdu_size);
}

int get_confirmation_frame()
{
    receive_frame();
}

int check_confirmation_frame()
{
    int is_error = 0;
    for (int i = 0; i < pdu_size; i++) {
        if (incoming_frame_buffer[i] == 0xFF)
            is_error = 1;
    }
    if (is_error)
        printf("%sReceived an ERROR confirmation from confirmation frame.\n", dll_warning_msg_format);

    return is_error;
}

int receive_frame()
{
    int result = receive_data_through_socket(incoming_frame_buffer, pdu_size);
    if (verbose) {
        printf("%sReceiving in: ", dll_info_msg_format);
        show_buffer(incoming_frame_buffer, pdu_size);
    }
    return result;
}

void get_frame_from_sender()
{
    if (verbose)
        printf("%sReceiving frame %lld.\n", dll_info_msg_format, incoming_frame_id);

    while (receive_frame())
        printf("%sFailed to receive frame %lld. Trying again...\n", dll_error_msg_format, incoming_frame_id);

    if (check_incoming_frame())
        while (send_error_confirmation_frame())
            printf("%sFailed to send ERROR confirmation frame. Trying again...\n", dll_error_msg_format);

    else
        while (send_ok_confirmation_frame())
            printf("%sFailed to send OK confirmation frame. Trying again...\n", dll_error_msg_format);
}

void pack_message_from_queue_buffer()
{
    *((long long *) outcoming_frame_buffer) = outcoming_frame_id++;
    for (int i = 0; i < pdu_size - PDU_HEADER_SIZE; i++)
        outcoming_frame_buffer[i + PDU_HEADER_SIZE] = queue_buffer[queue_buffer_pos++];
}

void unpack_message_from_frame_buffer()
{
    for (int i = 0; i < pdu_size - PDU_HEADER_SIZE; i++)
        queue_buffer[queue_buffer_pos++] = incoming_frame_buffer[i + PDU_HEADER_SIZE];
}