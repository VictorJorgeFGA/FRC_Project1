#include "SocketConnection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include <errno.h>

static int socket_data;

// VARIAVEIS PARA SOQUETE
struct sockaddr_in receiver_data;
struct sockaddr_in host_data;

void initialize_socket(char * host_address, char * host_port, char * receiver_address, char * receiver_port)
{
    receiver_data.sin_family = AF_INET;
    receiver_data.sin_addr.s_addr = inet_addr(receiver_address);
    receiver_data.sin_port = htons(atoi(receiver_port));

    host_data.sin_family = AF_INET;
    host_data.sin_addr.s_addr = inet_addr(host_address);
    host_data.sin_port = htons(atoi(host_port));

    socket_data = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_data < 0) {
        printf("Unable to open socket\nExiting...\n");
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }

    if (bind(socket_data, (struct sockaddr *) &host_data, sizeof(host_data)) < 0) {
        printf("Unable to bind socket to host port\nExiting...\n");
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }

    printf("Host socket initilized successfully\n");
    printf(
        "{UDP, Host IP: %s, Host port: %u, Receiver IP: %s, Receiver port: %u\n",
        inet_ntoa(host_data.sin_addr),
        ntohs(host_data.sin_port),
        inet_ntoa(receiver_data.sin_addr),
        ntohs(receiver_data.sin_port)
    );
}

void shut_down_socket()
{
    close(socket_data);
}

int send_data_through_socket(char * data_buffer, int data_buffer_size)
{
    int bytes_sent_amount = sendto(socket_data, data_buffer, data_buffer_size, 0,(struct sockaddr *) &receiver_data, sizeof(receiver_data));
    return bytes_sent_amount != data_buffer_size;
}

int receive_data_through_socket(char * data_buffer, int data_buffer_size)
{
    int receiver_data_size = sizeof(receiver_data);
    int bytes_read_amount = recvfrom(socket_data, data_buffer, data_buffer_size, 0, (struct sockaddr *) &receiver_data, &receiver_data_size);
    return bytes_read_amount != data_buffer_size;
}