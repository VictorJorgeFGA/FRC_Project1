#include "DataLinkLayer.h"
// #include "SocketConnection.h"

// TODO podar includes
#include "Application.h"
#include "DataLinkLayerInterface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_help()
{
    printf("\033[0;31mUsage\033[0m:\n\tStart Data Link Layer: \033[0;33mprog dll HOST_PORT RECEIVER_ADDRESS RECEIVER_PORT [options]\033[0m\n");
    printf("\n\t\tOptions:\n\t\t\t\033[0;35m[-p, --pdu-size] PDU_SIZE\033[0m\t\t\033[0;34m# Set the PDU size to PDU_SIZE (PS: this option is just a hint and might not take effect)\033[0m\n");
    printf("\t\t\t\033[0;35m[-v, --verbose]\t\t\t\t\033[0;34m# Start DLL in verbose mode\033[0m\n");
    printf("\n\tStart the Application: \033[0;33mprog app\033[0m\n\n");
}

int main(int argc, char ** argv)
{
    if (argc == 1) {
        show_help();
        return 0;
    }
    else if (argc > 1) {
        if (strcmp(argv[1], "app") == 0) {
            initialize_dll_interface(NANO_TIMEOUT);
            run_app();
            shut_down_dll_interface();
        }
        else if (strcmp(argv[1], "dll") == 0 && argc >= 5) {

            int argi = 5;
            int pdu_size = 100;
            int verbose = 0;
            while (argi < argc) {
                if (strcmp(argv[argi], "-p") == 0 || strcmp(argv[argi], "--pdu-size") == 0) {
                    if (argc < 7) {
                        show_help();
                        return 0;
                    }
                    pdu_size = atoi(argv[argi + 1]);
                    argi += 2;
                }
                else if (strcmp(argv[argi], "-v") == 0 || strcmp(argv[argi], "--verbose") == 0) {
                    verbose = 1;
                    argi++;
                }
            }
            initialize_dll(argv[1], argv[2], argv[3], pdu_size, verbose);
        }
        else {
            show_help();
        }
    }
    return 0;
}