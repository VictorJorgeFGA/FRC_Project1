#include "DataLinkLayerInterface.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

static mqd_t up_queue;
static mqd_t down_queue;

void initialize_dll_interface()
{
    struct mq_attr up_queue_properties;
    up_queue_properties.mq_maxmsg = CQ_MAX_DATA_AMOUNT;
    up_queue_properties.mq_msgsize = CQ_DATA_MAX_LEN;

    up_queue = mq_open(
        "/project_dll_up_queue",
        O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
        &up_queue_properties
    );

    if (up_queue < 0) {
        printf("An error occurred at up_queue creation\n");
        printf("%s")
    }

    struct mq_attr down_queue_properties;
    down_queue_properties.mq_maxmsg = CQ_MAX_DATA_AMOUNT;
    down_queue_properties.mq_msgsize = CQ_DATA_MAX_LEN;
}