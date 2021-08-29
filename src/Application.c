#include "DataLinkLayerInterface.h"
#include "Application.h"
#include <stdio.h>
#include <stdlib.h>

void process_file(char *file_path, int chunk_size)
{
    FILE *fp;

    fp = fopen(file_path, "r");

    if (fp == NULL) {
        //Tratar erro de forma eficiente
        printf("Error when opening file!\n");
        exit(0);
    }

    int file_size = 0;

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);

    rewind(fp);

    int payloads = file_size/chunk_size;
    char payload[chunk_size];

    while (fgets(payload, chunk_size, fp) != NULL) {
        printf("chunk: %s\n", payload);
        send_data_to_dll(payload, chunk_size);
    }

    fclose(fp);
}

void mount_file(char *filename, int chunk_size)
{

}