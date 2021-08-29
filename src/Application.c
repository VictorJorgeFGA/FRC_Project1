#include "DataLinkLayerInterface.h"
#include "Application.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void process_file(char *file_path, int chunk_size)
{
    FILE *fp;

    printf("%s\n", file_path);
    fp = fopen(file_path, "r");

    if (fp == NULL) {
        //Tratar erro de forma eficiente
        printf("%s\n", strerror(errno));
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

void mount_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w");

    char chunk_data[CQ_DATA_MAX_LEN];
    int chunk_len;
    int file_is_mounted = 0;

    while(!file_is_mounted) {
        get_data_from_dll(chunk_data, &chunk_len);

        for (int i = 0; i < chunk_len; i++){
            fwrite(chunk_data, chunk_len, 1, fp);
            file_is_mounted = (chunk_data[i] == EOF);
        }
    }

    fclose(fp);

}