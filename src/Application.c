#include "DataLinkLayerInterface.h"
#include "Application.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void process_file(char *file_path)
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

    char payload[CQ_DATA_MAX_LEN];

    memset(payload, EOF, sizeof(payload));
    int bytes_read;
    while (bytes_read = fread(payload, sizeof(char), CQ_DATA_MAX_LEN, fp)) {
        printf("chunk: %s\n", payload);
        send_data_to_dll(payload, CQ_DATA_MAX_LEN);
        memset(payload, EOF, sizeof(payload));
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
        get_data_from_app(chunk_data, &chunk_len);
        printf("Processing chunk: %s\n", chunk_data);

        for (int i = 0; i < chunk_len; i++) {
            if (chunk_data[i] == EOF) {
                file_is_mounted = 1;
                break;
            }
            fputc(chunk_data[i], fp);
        }
    }

    fclose(fp);
}