// #include "DataLinkLayer.h"
// #include "SocketConnection.h"

// #include "Application.h"
// #include "DataLinkLayerInterface.h"
// #include "Params.h"
#include <stdio.h>
#include <string.h>

// TODO REMOVER INCLUDES
// #include <time.h>
// #include <mqueue.h>

int main(int argc, char ** argv)
{
    // initialize_dll_interface();

    // process_file("file.txt");
    // mount_file("file_copy.txt");

    // shut_down_dll_interface();

    char msg[5000];
    *((int*)msg+200) = -9099090;
    int x = *((int*)msg+200);
    // unsigned int x = *((unsigned int *) msg);

    printf("%d\n", x);

    // printf("PDU_SIZE = %d\n", PDU_SIZE);
    // printf("MESSAGE_PDU_SIZE = %d\n", MESSAGE_PDU_SIZE);

    return 0;
}