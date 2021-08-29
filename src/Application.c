#include "Application.h"
#include <stdio.h>
#include <stdlib.h>

void process_message(char *file_path, int payload_size)
{
    FILE *fp;

    fp  = fopen(file_path, "r");

    if (fp == NULL) {
        //Tratar erro de forma eficiente
        printf("Error when opening file!\n");
        exit(0);
    }

    int file_size = 0;

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);

    rewind(fp);

    int payloads = file_size/payload_size;
    char payload[payload_size];

    while (fgets(payload, payload_size, fp) != NULL)


    fclose(fp);


}