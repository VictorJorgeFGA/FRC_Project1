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

    char chunk[CQ_DATA_MAX_LEN];

    int bytes_read;
    while (bytes_read = fread(chunk + CQ_HEADER_LEN, sizeof(char), CQ_MESSAGE_LEN, fp)) {
        printf("Reading chunk...\n");
        *((int *)chunk) = bytes_read;
        send_data_to_dll(chunk, CQ_DATA_MAX_LEN);
    }
    memset(chunk, 0x0, CQ_DATA_MAX_LEN);
    send_data_to_dll(chunk, CQ_DATA_MAX_LEN);

    fclose(fp);
}

void mount_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w");

    char chunk_data[CQ_DATA_MAX_LEN];
    int chunk_len;

    while(1) {
        get_data_from_app(chunk_data, &chunk_len);
        printf("Processing chunk...\n");

        int useful_msg_len = *((int *)chunk_data);
        if (useful_msg_len == 0)
            break;

        for (int i = CQ_HEADER_LEN; i < CQ_HEADER_LEN + useful_msg_len; i++) {
            printf("%c", chunk_data[i]);
            fputc(chunk_data[i], fp);
        }
    }

    fclose(fp);
}