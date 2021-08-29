#include "Application.h"
#include <stdio.h>
#include <stdlib.h>

void process_message(char *file_path, int PDU_size)
{
    FILE *fp;

    fp  = fopen(file_path, "r");
    int file_size = 0;

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);

    rewind(fp);

    if (fp == NULL) {
        //Tratar erro de forma eficiente
        printf("Error when opening file!\n");
        exit(0);
    }

    int frames = file_size/PDU_size;

    fclose(fp);


}