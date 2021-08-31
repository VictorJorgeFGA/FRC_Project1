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
            char cmd[150];
            while (1)
            {
                printf("Enter the file name to be sent, or \\exit to exit: ");
                scanf("%s", cmd);

                if (cmd[0] == '\\')
                    break;
                else
                    process_file(cmd);
            }
        } else {
            while (1) {
                char cmd[150];
                printf("Enter the file name to be received, or \\exit to exit: ");
                scanf("%s", cmd);

                if (cmd[0] == '\\')
                    break;
                else
                    mount_file(cmd);
            }
        }
        shut_down_dll_interface();
    }

    return 0;
}