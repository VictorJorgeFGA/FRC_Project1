#include "DataLinkLayerInterface.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static mqd_t up_queue;
static mqd_t down_queue;

static struct mq_attr up_queue_properties;
static struct mq_attr down_queue_properties;

static char * up_queue_name = "/up_queue";
static char * down_queue_name = "/down_queue";

void initialize_dll_interface()
{
    up_queue_properties.mq_maxmsg = CQ_MAX_DATA_AMOUNT;
    up_queue_properties.mq_msgsize = CQ_DATA_MAX_LEN;

    up_queue = mq_open(
        up_queue_name,
        O_CREAT | O_RDWR,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
        &up_queue_properties
    );

    if (up_queue < 0) {
        printf("An error occurred at up_queue creation\n");
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }

    down_queue_properties.mq_maxmsg = CQ_MAX_DATA_AMOUNT;
    down_queue_properties.mq_msgsize = CQ_DATA_MAX_LEN;

    down_queue = mq_open(
        down_queue_name,
        O_CREAT | O_RDWR,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
        &down_queue_properties
    );

    if (down_queue < 0) {
        printf("An error occurred at down_queue creation\n");
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
}

void shut_down_dll_interface()
{
    mq_close(down_queue);
    mq_close(up_queue);
    mq_unlink(down_queue_name);
    mq_unlink(up_queue_name);
}

void send_data_to_dll(char * data, int data_len)
{
    if (mq_send(down_queue, data, data_len, 0) < 0) {
        printf("An error occurred when sending data to dll\n");
        printf("Error: %s\n", strerror(errno));
    }
}

void get_data_from_dll(char * data, int * data_len)
{
    int priority;
    int bytes_received = mq_receive(up_queue, data, CQ_DATA_MAX_LEN, &priority);
    if (bytes_received < 0) {
        printf("An error occurred when receiving data from dll\n");
        printf("Error: %s\n", strerror(errno));

        return;
    }
    *data_len = bytes_received;
}

void send_data_to_app(char * data, int data_len)
{
    if (mq_send(up_queue, data, data_len, 0) < 0) {
        printf("An error occurred when sending data to app\n");
        printf("Error: %s\n", strerror(errno));
    }
}

void get_data_from_app(char * data, int * data_len)
{
    int priority;
    int bytes_received = mq_receive(down_queue, data, CQ_DATA_MAX_LEN, &priority);
    if (bytes_received < 0) {
        printf("An error occurred when receiving data from app\n");
        printf("Error: %s\n", strerror(errno));

        return;
    }
    *data_len = bytes_received;
}