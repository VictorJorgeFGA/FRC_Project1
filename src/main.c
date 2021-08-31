#include "DataLinkLayer.h"
// #include "SocketConnection.h"

// TODO podar includes
#include "Application.h"
#include "DataLinkLayerInterface.h"
#include "Params.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv)
{
    if (argc > 2) {
        initialize_dll(argv[5][0] == 'S' ? SENDER : RECEIVER, argv[1], argv[2], argv[3], argv[4]);
        run_dll();
        shut_down_dll();
    } else {
        initialize_dll_interface();
        if (argv[1][0] == 'S') {
            process_file("file.mp3");
        } else {
            mount_file("file_copy.exe");
        }
        shut_down_dll_interface();
    }

    return 0;
}